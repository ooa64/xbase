#!/usr/bin/perl

################################################################################
# Program	: xb2cpp.pl V0.1 26-11-98
#
# Author	: Hubertus Kehl,  kehl@informatik.tu-muenchen.de
#                                 kehlh@hotmail.com
# Language      : Perl 5.0  
#
# Purpose	: Convert Dbase, Clipper, FOXPRO Source Code to XBASE C++ Code
#                 for use with Gary Kunkel's XBASE DBMS Library
#
# Output	: .cpp File and .h File
#
# Arguments	: 1st arg = filename
################################################################################
#
# Remarks	: The script tries to keep track of the databases used in
#                 different workareas. But the flow through your program
#                 cannot be determined, so in some cases you have to help
#                 the script a bit by giving additional info.
#                 Three special comments have been designed to add this info:
#                 * XB2CPP(SelectedArea)
#          or     * XB2CPP(SelectedArea,Workarea1,Alias1,Wa2,Al2...)
#          or     * XB2CPP(SelA,WA1,Al1[Total1, Active1],Wa2, Al2[Tot2,Act2])
#                 e.g. * XB2CPP(3,3,ZP,2,PL) will inform the script for example
#                 at the beginning of a PROCEDURE that a database with Alias
#                 ZP is opened in workarea 3 and a database with alias PL is
#                 opened in workarea2 and when calling this procedure always
#                 workarea 3 is selected.
#                 The third alternative of this statement makes it possible
#                 to define the number of open indices and which one is the
#                 active index, as given by a previous SET INDEX TO or USE
#                 and SET ORDER TO statements. The script cannot know, which
#                 procedures, statements have been executed before and opened
#                 indices.... So * XB2CPP(2,2,ZP[2,1]) will inform the script
#                 about an open database alias ZP in workarea 2 which is
#                 currently selected and has two open indices where the first
#                 one is the active one.
# 
# Hints        :  1. Always give an ALIAS in USE-Statements this is needed for
#                 generation of the Database Classnames
#                 2. Give the above described additional infos where necessary.
#                 3. Use ALIASes in Expressions with database fields, this
#                 helps the script in knowing where database references are.
#                 4. Replace the XXX in PutXXXField or GetXXXField manually
#                 the script is not able to determine variable or field types.
#                 The code for getting a Textfield (GetField) must also be
#                 corrected. Xbase DBMS Syntax requires the buffer as second
#                 parameter, the string will not be returned !
#                 You may use a C++ String wrapper for this job. 
#                 5. PROCEDURES that will operate on two different databases
#                 where some statements are used for both cannot be converted
#                 The script needs to know everytime on which database the 
#                 statements are operating. Either duplicate such procedures,
#                 (one for each database), or write a generic C++ routine with
#                 pointers to databases.
#                 6. Don't assume any compilable or even working C++ source code
#                 as output. The ambigous DBASE language, which lacks type and
#                 variable definitions is one reason for this. This script only
#                 will help you for about 10-30% of the conversion. The rest
#                 (which remained uppercase) has to be done manually :) 
#                 7. Please send feedback, patches, suggestions etc. to one
#                 of the above named email adresses.
#		 
################################################################################
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation (version 2 of the License)
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
################################################################################

# Setup your tabsize here
$tabsize=4;
# workarea 1 is selected by default
$selected=1;
# table to get the alias name with a given workarea
@dbftab=();
# returns the number of open indices in a workarea
@nropenindices=();
# LoL of openindices, [workarea][1..7], returns C++ name of index
@openindices=();  
# returns the number of the active index in a workarea for find and seek
@activeindices=();

#Hash table for getting the workarea with the alias name
%dbfhash=();
$dbftab[0]="";
#Hash table for gathering all C++ DBF names
%totaldbfnames=();
#Hash table for gathering all C++ NDX names
#names will be counted for each associated database, Index 10 is for creating
%totalndxnames=();

if ($#ARGV != 0)
{   die "$0: 'Usage: perl xb2cpp.pl inputfile\nGenerates inputfile.cpp and inputfile.h\nUse additional comments in the inputfile to guide the conversion:\n* XB2CPP(SelectedArea, Workarea1, Alias1, Wa2, Al2, Wa3, Al3, ...)\n* XB2CPP(SelectedArea)\n* XB2CPP(SelA,WA1,Al1[Total1, Active1],Wa2, Al2[Tot2,Act2])\n will inform the script about open databases/indices and/or selections.\nSee the script header for more information'\n";
}

open (INPUT, "<$ARGV[0]")	|| die "$0: 'can't open input file $ARGV[0]'\n";

$outname=$ARGV[0];
$houtname=$ARGV[0];
$outname=~ s/\.prg/.cpp/;
$houtname=~ s/\.prg/.h/;

$cppbuf.="// C++ Generated by xb2cpp.pl V0.1 (C) 1998 by Hubertus Kehl\n";
$cppbuf.="// xb2cpp.pl is free software under the GNU General Public License\n\n";
$cppbuf.="#include \"".$houtname."\"\n";
$hbuf.="// C++ Header generated by xb2cpp.pl V0.1 (C) 1998 by Hubertus Kehl\n";
$hbuf.="// xb2cpp.pl is free software under the GNU General Public License\n\n";
$hbuf.="#include <xbase/xbase.h>\n";

($classname,$rest)=split /\./,$ARGV[0],2;
$classname=nameid($classname);

$hbuf.="class $classname\n{\n    public:\n";
$hbuf.="    SHORT rc;\n";
$hbuf.="    SHORT grc;\n";
$hbuf.="    SHORT frc;\n";

$proto.="    // Function Prototypes\n";
$proto.="    ".$classname."();\n";
$foundproc=0;

# count the lines
$linenr=0;
print "Analyzing ...\n";
while (<INPUT>)
{   $linenr++; 
    #convert the tabs to spaces   
    $_=expandline($_);

    # is there a Comment ?
    if (!(m/^\s*\*/))
    {   # if no convert everything to uppercase
        $_=~tr/a-z/A-Z/;
        $line=$_;
    }
    else
    {
        $line=$_;
        $indexc=index($_,"*");
        $indentc=" " x $indexc;
        $indentc=$indentc."//";
        $line=~ s/^\s*\*/$indentc/;
        if ($foundproc==1)
        {   # indent if inside a procedure
            $line="    ".$line;
        }
        # check for special comment
        $selected=xb2cppcomm($line, $linenr,$selected);        
        $cppbuf.=$line;
        # skip to end of while 
        next;
    };
 
    # does the line contain an ALIAS->RECNO()
    if ($line=~m/(\w*)\s*-\>\s*RECNO\s*\(\s*\)/)
    {
         $repl=nameid($1).".GetCurRecNo()";
         $line=~s/(\w*)\s*-\>\s*RECNO\s*\(\s*\)/$repl/;
    }
    # does the line contain a RECNO()
    if ($line=~m/RECNO\s*\(\s*\)/)
    {
         $name=get_dbf($linenr,$selected);
         $repl=$name.".GetCurRecNo()";
         $line=~s/RECNO\s*\(\s*\)/$repl/;
    }
    # does the line contain an ALIAS->RECCOUNT()
    if ($line=~m/(\w*)\s*-\>\s*RECCOUNT\s*\(\s*\)/)
    {
         $repl=nameid($1).".NoOfRecords()";
         $line=~s/(\w*)\s*-\>\s*RECCOUNT\s*\(\s*\)/$repl/;
    }
    # does the line contain a RECCOUNT()
    if ($line=~m/RECCOUNT\s*\(\s*\)/)
    {
         $name=get_dbf($linenr,$selected);
         $repl=$name.".NoOfRecords()";
         $line=~s/RECCOUNT\s*\(\s*\)/$repl/;
    }

    # did we find a replace with, but without FOR and WHILEs ?
    if (($line=~m/^\s*REPLACE.*WITH.*$/) 
        && (!($line=~m/^.*WITH.*\sFOR.*$/))
        && (!($line=~m/^.*WITH.*\sWHILE.*$/)))
    {
        if ($line=~m/REPLACE\s*(\w*)\s*-\>\s*(\w*)\s*WITH(.*)$/)
        {
              $name=nameid($1);
              $field=$2;
              $expr=$3;
        }
        else
        {
            if ($line=~m/REPLACE\s*(\w*)\s*WITH(.*)$/)
            {     $field=$1;
                  $expr=$2; 
                  $name=get_dbf($linenr,$selected);
            }
        }
        $spaces=get_indent($line);
        $cppbuf.=$spaces.$name.".PutXXXField(\"".$field."\",".$expr.");\n";
        next;
    }

    # did we find GO|GOTO TOP ?
    if ($line=~m/^\s*(GOTO|GO)\s*TOP\s*$/) 
    {
         $name=get_dbf($linenr,$selected);
         $spaces=get_indent($line);
         $cppbuf.=$spaces."grc=".$name.".GetFirstRecord();\n";
         next;
    }
    
    if ($line=~m/^\s*(GOTO|GO)\s*BOTTOM\s*$/) 
    {
         $name=get_dbf($linenr,$selected);
         $spaces=get_indent($line);
         $cppbuf.=$spaces."grc=".$name.".GetLastRecord();\n";
         next;
    }
    # did we find GO|GOTO expr
    if ($line=~m/^\s*(GOTO|GO)\s+(.*)$/) 
    { 
         $name=get_dbf($linenr,$selected);
         $spaces=get_indent($line);
         $cppbuf.=$spaces."grc=".$name.".GetRecord($2);\n";
         next;
    }

    # did we find a SKIP or SKIP 1
    if (($line=~m/^\s*SKIP\s*$/) || ($line=~m/^\s*SKIP\s*1\s*$/))
    { 
         $name=get_dbf($linenr,$selected);
         $spaces=get_indent($line);
         $cppbuf.=$spaces."grc=".$name.".GetNextRecord();\n";
         next;
    }

    # did we find a SKIP -1
    if ($line=~m/^\s*SKIP\s*-\s*1\s*$/)
    { 
         $name=get_dbf($linenr,$selected);
         $spaces=get_indent($line);
         $cppbuf.=$spaces."grc=".$name.".GetPrevRecord();\n";
         next;
    }

    # gen code for all references with alias
    if ($line=~m/(\w*)\s*-\>\s*(\w*)/)
    {   
        $line=gen_aliasref($1,$2,$line);
    }
 
    $condition=0;
    if (($line=~m/\s*ELSE/) || ($line=~m/ELSE\n/) || ($line=~m/ELSE\s*/)) 
    {
        $indent=calcindent($line);
        if ($foundproc==1)
        {
            $repl=$indent."}\n    ".$indent."else\n    ".$indent."{";
        }
        else
        {
            $repl=$indent."}\n".$indent."else\n".$indent."{";
        }
        $line=~s/\s*ELSE/$repl/;
    };
    # SELECT statement
    if (($line=~m/^\s*SELECT\s/)) 
    {
        $selected=selectstat($line,$linenr);
        #generate no code for select
        next;
    }

    # USE statement (should have an alias)
    if (($line=~m/^\s*USE\s/)) 
    {   
        if ($selected==0)
        {
             die "xb2cpp does not know which workarea is selected in line $linenr\nPlease give additional info with a comment like:\n* XB2CPP(SelectedArea, Workarea1,Alias1, Wa2, Al2 ...) or\n* XB2CPP(SelectedArea) before this line.\n"
        }

        # do we have to close the old one ? 
        if ($dbftab[$selected] ne "")
        {
            gen_closedbf($dbftab[$selected],$line);
        }
        #database closed, so the indices are too.
        $nropenindices[$selected]=0;
        # an USE alone ?
        if ($line=~m/^\s*USE\s*$/)
        {
            #remove entry
            $name=$dbftab[$selected];
            if ($name ne "")
            {
                $dbftab[$selected]="";
                $dbfhash{$name}=0;
            }
            next;
        }
        
        $alias="";
        $indexlist=""; 
        if ($line=~m/^\s*USE\s*(.*)\s*INDEX\s*(.*)\s*ALIAS\s*(\w*)\s*/)
        {   
            $name=$1;
            $indexlist=$2;
            $alias=$3;
        }
        else
        {
            if ($line=~m/^\s*USE\s*(.*)\s*ALIAS\s*(\w*)\s*/)
            {
                $name=$1;
                $alias=$2;
            }
            else
            {
                 die "No ALIAS given in line $linenr. xb2cpp uses the ALIAS for generating\n database class names. Please add an ALIAS to this USE statement.\n";
            }
        }
        # add/update entry
        $dbftab[$selected]=$alias;
        $dbfhash{$alias}=$selected; 
        $dbfname=nameid($alias);
        $totaldbfnames{$dbfname}=1;
        # generate handle index10 for possible creation of indexes
        $indexname=nameid($dbfname)."_I10";
        $totalndxnames{$indexname}=1;

        gen_use($selected,$alias,$name,$indexlist,$line);
        next;
    }

    $line=~s/ENDIF/}/;
    $line=~s/ENDDO/}/;

    if (($line=~m/.*IF\s/) || ($line=~m/.*DO\sWHILE/)) 
    {
        $condition=1;
        $indent=calcindent($line);
    };

    $line=~s/IF\s/if \(/;
    $line=~s/DO\sWHILE\s/while \(/;

    # we found a procedure ?
    if (($line=~m/^\s*PROCEDURE\s/))
    {   # delete the database knowledge/workareas
        # should be clear in every procedure itself !
        $selected=0;
        @dbftab=();
        %dbfhash=();
        # is this not the first one ?
        if ($foundproc==1)
        {   # close the last one
            $cppbuf.="}\n\n\n";
        }
        $foundproc=1;
        $line=~s/PROCEDURE\s*//;
        # remove spaces
        $line=~s/\s*//g;
        #C++ Style
        $line=nameid($line)."()";
        $hline="    ".$line.";\n";
        $line.="\n{\n";
        # add class definition to header
        $proto.=$hline;
        $cppbuf.=$classname."::".$line;
        next;
    }
    # did we find an append blank ?
    if (($line=~m/^\s*APPEND\s*BLANK\s*$/) || ($line=~m/^\s*APPEND\s*$/))
    {
        $name=get_dbf($linenr,$selected);
        $spaces=get_indent($line);
        $cppbuf.=$spaces."if ((rc=$name.AppendRecord()) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError appending data record rc = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";
        next;
    }
    # did we find a single delete ?
    if ($line=~m/^\s*DELETE\s*$/)
    {
        $name=get_dbf($linenr,$selected);
        $spaces=get_indent($line);
        $cppbuf.=$spaces."if ((rc=$name.DeleteRecord()) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError deleting data record rc = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";
        next;
    }

    # did we find a delete all?
    if ($line=~m/^\s*DELETE\s*ALL\s*$/)
    {
        $name=get_dbf($linenr,$selected);
        $spaces=get_indent($line);
        $cppbuf.=$spaces."if ((rc=$name.DeleteAllRecords()) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError deleting data records rc = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";
        next;
    }
    
    # did we find a recall all?
    if ($line=~m/^\s*RECALL\s*ALL\s*$/)
    {
        $name=get_dbf($linenr,$selected);
        $spaces=get_indent($line);
        $cppbuf.=$spaces."if ((rc=$name.UndeleteAllRecords()) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError undeleting data records rc = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";
        next;
    }

    # did we find a pack ?
    if ($line=~m/^\s*PACK\s*$/)
    {
        $name=get_dbf($linenr,$selected);
        $spaces=get_indent($line);
        $cppbuf.=$spaces."if ((rc=$name.PackDatabase(F_SETLK)) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError packing database rc = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";
        next;
    }
    # did we find a procedure call ?
    if ($line=~m/^\s*DO(.*)WITH(.*)$/)
    {
         $name=$1;
         $params=$2;
         $spaces=get_indent($line);
         $name=~s/\s*//g;
         $name=nameid($name);
         $params=~s/^\s*//; 
         $params=~s/\s*$//;
         $cppbuf.=$spaces.$name."(".$params.");\n";
         next;
    }
    # did we find a close ?
    if ($line=~m/^\s*CLOSE\s*(\w*)$/)
    {
        $what=$1;
        $what=~s/\s//g;
        if (($what=~m/DATABASES/) || ($what=~m/ALL/)) 
        {    gen_closealldbf($line);
             next;
        }
        if ($what=~m/INDEX/) 
        {    gen_closeallndx($selected,$line);
             next;
        }
    }
    # did we find a set index to?
    if ($line=~m/^\s*SET\s+INDEX\s+TO\s*(.*)$/)
    {
        $indexlist=$1;
        $indexlist=~s/\s//g;
        if ($indexlist ne "") 
        {
            gen_openindex($selected,$indexlist,$line);
        }
        else
        {    
            gen_closeallndx($selected,$line);
        }
        next;
    }

    # did we find a index on ?
    if ($line=~m/^\s*INDEX\s+ON\s*(.*)\s*TO\s*(.*)$/)
    {
          $expr=$1;
          $rest=$2;
          $spaces=get_indent($line);
          if ($rest=~m/^\s*(.*)\s*UNIQUE\s*$/)
          {
              $name=$1;
              $unique=1;
          }
          else
          {
              $name=$rest;
              $unique=0;
          }
          if ($name=~m/^\&/)
          { 
              $name=~s/\&//; 
          }
          else
          {
              $name="\"$name\"";
          }
          $expr="\"$expr\"";
          $indexname=nameid($dbftab[$selected])."_I10";
          $cppbuf.=$spaces."if ((rc=$indexname.CreateIndex($name,$expr,$unique,1)) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError creating index = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";
          #close it    
          gen_closendx($indexname, $line);
          next;
    }   
 
    # did we find a set order to?
    if ($line=~m/^\s*SET\s+ORDER\s+TO\s*(\d)$/)
    {
         $activeindices[$selected]=$1;
         next;
    }

    # did we find a FIND or SEEK?
    if ($line=~m/^\s*(FIND|SEEK)\s+(.*)$/)
    {
        if ((!($activeindices[$selected])) || ($activeindices[$selected]==0))
        {
             die "No knowledge about active indices for find/seek in Line $linenr. Use XB2CPP comment to add this info before this line.\n";
        }
        $expr=$2;
        $expr=~s/[\"\'\[\]\s]//g;
        $active=$activeindices[$selected];
        $indexname=$openindices[$selected][$active];
        $spaces=get_indent($line);
        # hope its alphanumeric ?
        $cppbuf.=$spaces."//alpha search ?\n".$spaces."frc=$indexname.FindKey(\"".$expr."\");\n".$spaces."//Evaluate frc of find here\n";
        next;
    }

    if ($condition==1)
    {
        # replace some operators 
        $line=~ s/.AND./&&/g;
        $line=~ s/.OR./||/g;
        $line=~ s/.NOT./!/g;
        $line=~ s/\<\>/!=/g;
        $line=~s/\=/==/g;
        if ($foundproc==1)
        {
            $repl="\)\n    ".$indent."\{\n";
        }
        else
        {
            $repl="\)\n".$indent."\{\n";
        }
        $line=~s/\n/$repl/;
    };
    
    if ($foundproc==1)
    {   # indent if inside a procedure
        $line="    ".$line;
    }
    $cppbuf.=$line;
}
close (INPUT);		# Close input file

if ($foundproc==1)
{   # close the last procedure
    $cppbuf.="}\n";
}

print "Writing C++ File...\n";
$cppbuf.="// Constructor\n";
$cppbuf.=$classname."::".$classname."()\n{\n";
$cppbuf.="    Xbase x;\n";
$cppbuf.="    // DBFs\n";
foreach $dbfname (keys %totaldbfnames)
{
   $cppbuf.="    DBF ".$dbfname."(&x);\n";
}
$cppbuf.="    // NDXs\n";
# write the index class definitions
foreach $indexname (keys %totalndxnames)
{
    chomp $indexname;
    ($firstpart,$rest)=split(/_/,$indexname);
    $cppbuf.="    NDX ".$indexname."(&".nameid($firstpart).");\n";
}
$cppbuf.="}\n";

open (OUTPUT, ">$outname") || die "$0: 'can't write to $outname ($!)'\n";
print OUTPUT $cppbuf;
close OUTPUT;

print "Writing C++ Headerfile...\n";
$proto.="};\n";
$hbuf.="    // DBFs\n";
foreach $dbfname (keys %totaldbfnames)
{
   $hbuf.="    DBF ".$dbfname.";\n";
}
$hbuf.="    // NDXs\n";
foreach $indexname (keys %totalndxnames)
{
    chomp $indexname;
    $hbuf.="    NDX ".$indexname.";\n";
}
$hbuf.=$proto;

open (OUTPUT, ">$houtname") || die "$0: 'can't write to $houtname ($!)'\n";
print OUTPUT $hbuf;
close OUTPUT;

# expand tabs
sub expandline
{
    $line=$_[0];
    while($line=~m/\t/)
    { 
        $pos=index($line,"\t");
        $newpos=(int($pos / $tabsize)+1)*$tabsize;
        $repl=" " x (($newpos-$pos));
        $line=~s/\t/$repl/;
    }
    return($line);
}

# name an identifier C++ like
sub nameid
{
    $cppname=$_[0];
    #lower case it
    $cppname=~tr/A-Z/a-z/;
    # first char upper case
    $cppfirst=substr($cppname,0,1);
    $cppfirst=~tr/a-z/A-Z/;
    return($cppfirst.substr($cppname,1,1000));
}

# calculate the current indentation
sub calcindent
{   
    $line=$_[0];
    $spaces="";
    #calculate the current indentation
    if ($line=~m/^(\s*)(\S*)/)
    {
        $spaces=$1;
    }
    return($spaces);
}

# a select command was found 
sub selectstat
{   
    $comp=$_[0];
    $nr=$_[1];
    $comp=~s/\s*SELECT\s*//;
    $comp=~s/\s//g;
    # a number was selected ?
    if ($comp=~m/^\d+$/)
    {   
        return($comp);
    }
    if (!($dbfhash{$comp}))
    {   
        # Selection of an unknown database
        die "xb2cpp does not know which workarea is selected in line $nr\nPlease give additional info with a comment like:\n* XB2CPP(SelectedArea, Workarea1,Alias1, Wa2, Al2 ...) before this line.\n"
    }
    else
    {
       return($dbfhash{$comp}); 
    }
}

# gets the currently selected database class name
sub get_dbf
{
    $nr=$_[0];
    $selected=$_[1];
    if ($dbftab[$selected] eq "")
    {
        die "xb2cpp does not know which database is opened in the current workarea in line $nr\nPlease give additional info with a comment like:\n* XB2CPP(SelectedArea, Workarea1,Alias1, Wa2, Al2 ...) or\n* XB2CPP(SelectedArea) before this line.\n"
    }
    $name=nameid($dbftab[$selected]);
    return($name);
}

# evaluate comment, look for definition of workareas and aliases (databases)
# if xb2cpp is given
sub xb2cppcomm
{
    $comp=$_[0];
    $nr=$_[1];
    $selected=$_[2];
    $comp=~tr/a-z/A-Z/;
    # we found a special XB2CPP comment
    if ($comp=~m/^.*XB2CPP(.*)$/)
    {   $comp=$1;
        $comp=~s/\(//g;
        $comp=~s/\)//g;
        $comp=~s/\s//g;
        # replace the , to ; in the index definitions
        $comp=~s/\[(\d+)\,(\d+)\]/\[$1\;$2\]/g;

        # is it just a selected workarea ? or a list ?
        if ($comp=~m/^\d+$/)
        {  
            return($comp);
        }
        # there is more than a single number
        @array=split(',',$comp);
        if ($#array <0)
        {
             die "Wrong number of parameters or incorrect index definition in line $nr in the XB2CPP comment\n";
        }

        #first element of array should give selected workarea
        $selected=shift @array;
        if (!($selected=~m/^\d+$/))
        {
             die "Selected workarea $first is not a number in line $nr in the XB2CPP comment\n";
        } 

        if (($#array % 2) != 1)
        {
             die "Wrong number of parameters in line $nr in the XB2CPP comment\n";
        }

        $gotit=0;  # two elements form an entry: selected, workarea1, alias1, workarea2...

        foreach $elem (@array)
        {    if ($gotit==0)
             {    # firstelem is workarea, should match only numbers
                  if ($elem=~m/^\d+$/)
                  {   
                      $workarea=$elem;
                  }
                  else 
                  {
                      die "Workarea $elem is not a number in line $nr in the XB2CPP comment\n";
                  }
                  $gotit=1;
             }    
             else
             {    #secondelem is alias and optional a indexdefinition
                  #is there a definition of open indices 
                  if ($elem=~m/^(\w+)\[(\d)\;(\d)\]$/)
                  {
                      $alias=$1;
                      $total=$2;
                      $active=$3;
                      if ($active >$total)
                      {
                            die "Error: active index > total index in line $nr in the XB2CPP comment\n";
                      }
                      $nropenindices[$workarea]=$total;
                      $activeindices[$workarea]=$active;
                      for($ii=1;$ii<=$total;$ii++)
                      {
                          $indexname=nameid($alias)."_I".($ii);
                          $openindices[$workarea][$ii]=$indexname;
                          $totalndxnames{$indexname}=1;
                      }
                      $dbftab[$workarea]=$alias;
                      $dbfhash{$alias}=$workarea; 
                      $gotit=0;
                  }
                  else
                  {
                      if ($elem=~m/^(\w+)$/)
                      {
                          if ($elem=~m/^(\d+)$/)
                          {
                              die "Alias $elem is only a number in line $nr in the XB2CPP comment\n";   
                          }
                          $alias=$elem;
                          $dbftab[$workarea]=$alias;
                          $dbfhash{$alias}=$workarea; 
                          $gotit=0;
                          # remove index entries for workarea                            
                          for($ii=1;$ii<=$nropenindices[$workarea];$ii++)
                          {
                               $openindices[$workarea][$ii]="";
                          }
                          $nropenindices[$workarea]=0;
                          $activeindices[$workarea]=0;
                      }
                      else
                      {
                          die "Error in Alias or Index definition $elem in line $nr in the XB2CPP comment\n";   
                      }
                  }
             }
        }
    }
    return($selected);
}

sub get_indent
{
    $line=$_[0];
    $spaces="";
    #calculate the current indentation
    if ($line=~m/^(\s*)(\S*)/)
    {
        $spaces=$1;
        if ($foundproc==1)
        {
            $spaces.="    ";
        }
    } 
    return($spaces); 
}

sub gen_closedbf
{
    $dbf=$_[0];
    $line=$_[1];
    $cppbuf.=get_indent($line).nameid($dbf).".CloseDataBase\(\)\;\n";
    #remove entry
    $wa=$dbfhash{$dbf};
    $dbftab[$wa]="";
    $dbfhash{$dbf}=0;
}

sub gen_closendx
{
    $index=$_[0];
    $line=$_[1];
    $cppbuf.=get_indent($line).$index.".CloseIndex\(\)\;\n";
}

sub gen_closealldbf
{
    $line=$_[0];
    for($id=1;$id<=$#dbftab;$id++)
    {
        if ($dbftab[$id] ne "")
        {
             gen_closedbf($dbftab[$id],$line);
        } 
    }
    @openindices=();  
    @nropenindices=();
    @activeindices=();
}

sub gen_closeallndx
{
    $selected=$_[0];
    $line=$_[1];
    
    for($ii=1;$ii<=$nropenindices[$selected];$ii++)
    {   $indexname=$openindices[$selected][$ii];
        if ($indexname ne "")
        {   gen_closendx($indexname,$line);
        }
        $openindices[$selected][$ii]="";
    }
    $nropenindices[$selected]=0;
    $activeindices[$selected]=0;

}

sub gen_use
{
    $selected=$_[0];
    $dbf=$_[1];
    $name=$_[2];
    $indexlist=$_[3];
    $line=$_[4];
   
    # add/update entry
    $dbftab[$selected]=$dbf;
    $dbfhash{$dbf}=$selected; 
    $dbfname=nameid($dbf);
    $totaldbfnames{$dbfname}=1;

    $indexlist=~s/\s//g;
    $name=~s/\s//g;

    $spaces=get_indent($line);
    if ($name=~m/^\&/)
    {
        $name=~s/\&//; 
    }
    else
    {
       $name="\"$name\"";
    }
    $id=nameid($alias);
    $cppbuf.=$spaces."if ((rc=$id.OpenDataBase($name)) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError opening file rc = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";

    gen_openindex($selected,$indexlist,$line);
} 

sub gen_openindex
{ 
    $selected=$_[0];
    $indexlist=$_[1];
    $line=$_[2];
  
    $dbfn=$dbftab[$selected]; 
    if ($indexlist ne "")
    {
        @indices=split(/\,/,$indexlist);
        $nropenindices[$selected]=$#indices+1;
        $activeindices[$selected]=1;
        for($ii=0;$ii<=$#indices;$ii++)
        {
            $indexname=nameid($dbfn)."_I".($ii+1);
            $openindices[$selected][$ii+1]=$indexname;
            $realname=$indices[$ii]; 
            if ($realname=~m/^\&/)
            {
                $realname=~s/\&//; 
            }
            else
            {
                $realname="\"$realname\"";
            }
            $totalndxnames{$indexname}=1;
            $spaces=get_indent($line);
            $cppbuf.=$spaces."if ((rc=$indexname.OpenIndex($realname)) != XB_NO_ERROR)\n".$spaces."{\n".$spaces."    cout << \"\\nError opening index rc = \"<< rc << \"\\n\";\n".$spaces."    exit(-1);\n".$spaces."}\n";
        }
    }
    else
    {
        $nropenindices[$selected]=0;
        $activeindices[$selected]=0;
    }
}

sub gen_aliasref
{  # does not work for reference to textfield because text is not returned
   # use C++ Wrapper for Strings for this purpose
   $alias=$_[0];
   $variable=$_[1];
   $line=$_[2];
   $repl=nameid($alias).".GetXXXField(\"".$variable."\")";
   $line=~s/(\w*)\s*-\>\s*(\w*)/$repl/;
   return($line);
}

