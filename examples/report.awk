# report.awk - sample XBase report awk script
#
BEGIN{ 
  printf( "\nXBase Sample AWK Report Script\n\n\n" );
  FS=",";
  RECCNT = 0;
  TOTAL  = 0;
}
# Main report loop
{
  NAME = $1;
  DATE = $3;
  AMT  = $4;
  

  if(( RECCNT % 50 ) == 0 ){
    printf( "Name             Date        Amount\n" );
    printf( "----------   -----------    --------\n" );
  }

  RECCNT++;
  printf( "%-10s   %8s      %5.2f\n", NAME, DATE, AMT );
  TOTAL = TOTAL + AMT;
}
END{
  printf( "                            --------\n" );
  printf( "Total                        %6.2f\n", TOTAL );

  printf( "\n\nEnd Of Report\n" );
}
