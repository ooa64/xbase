#! /bin/sh

# Heavy duty rename tool.  
# Copyleft 1998, Vitaly Fedrushkov <fedrushkov@acm.org>

# Renames all pre-1.8 Xbase symbols to their new names.
#
# Generates xbcompat.h that may be used instead.
#
# Dictionary is at xbase_rename.txt
#
#	Usage: xbase_rename.sh [file] ...

awk '
(length($1) > 0) { print "/\\<" $1 "\\>/\t{ gsub(\"\\\\<" $1 "\\\\>\", \"" $2 "\") }" }
($1 == "")	 { print }
END 		 { 
	print "/\\.xbDbf/ { gsub(\"\\\\.xbDbf\", \".DBF\") }"
	print "/\\.xbNdx/ { gsub(\"\\\\.xbNdx\", \".NDX\") }"
	print "{ print }" 
}
' < xbase_rename.txt > xbase_rename.awk || exit 1

for i in $*; do
    echo $i
    cp -a $i $i.orig~ && \
    awk -f xbase_rename.awk < $i.orig~ > $i
done

cat <<EOF > xbcompat.h
/* Generated automagically from xbase_rename.txt by xbase_rename.sh. */
/* Do not edit by hand. */

#ifndef	__XB_COMPAT_H__
#define	__XB_COMPAT_H__

EOF

awk '
(length($1) > 0) { printf "#define\t%-24s%s\n", $1, $2 }
($1 == "")	 { print }
END 		 { printf "\n#endif\t// __XB_COMPAT_H__\n" }
' < xbase_rename.txt | unexpand -a >> xbcompat.h
