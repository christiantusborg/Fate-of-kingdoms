#!/usr/bin/perl

print "vnum.pl areaname.are start_vnum end_vnum new_starting_vnum\n";
print "must be run in the same directory as the file.\n";
print "please note, currently does not check exits that lead out of area.\n";
open(VNUMFILE, $ARGV[0])||die "ERROR OPEING FILE $!\n";
open(TEMPFILE, ">areafile");
open(ERRORFILE, ">error");
print "beggining run\n";
$count=1;
$oldstart = $ARGV[1];
$oldend = $ARGV[2];
$newstart = $ARGV[3];
$offset = $newstart - $oldstart;
$newend = $oldend+$offset;
$position = 0;
$testhash="#";
while ($line= <VNUMFILE>){

if ($count==4)
	{
		@list=split(" ",$line);
		if (@list[1]!=$oldstart)
			{print "\npossible error, starting vnum dosent match\n";}
		if (@list[2]!=$oldend)
			{print "\npossible error, ending vnum dosent match\n";}
		@list=("VNUMs",$newstart, $newend,"\n");
		$line = join(" ",@list);
	}

@linelist=split("",$line);

if (@linelist[0] eq $testhash)
{
if ($line =~ /[#]AREA/)
{$position=1;}
if ($line =~ /[#]MOBILES/)
{$position=2;}
if ($line =~ /[#]OBJECTS/)
{$position=3;}
if ($line =~ /[#]ROOMS/)
{$position=4;}
if ($line =~ /[#]RESETS/)
{$position=5;}
if ($line =~ /[#]SPECIALS/)
{$position=6;}
if ($line =~ /[#]SHOPS/)
{$position=7;}
}




if ($position>1)
{
if(@linelist[0] eq $testhash)
{

	@words = split(/#/,$line);
	foreach $tempvar (@words)
	{
		chomp($tempvar);
		if (($tempvar>=$oldstart)&&($tempvar<=$oldend))
			{
		 	 $tempvar=$tempvar+$offset;
			$line = join("","#",$tempvar,"\n");
			}
	}

}
if ((@linelist[0] eq "M")&&($position<5))
{
$i=0;
 	@words = split(/ /,$line);
	foreach $tempvar(@words)
	{
		chomp ($tempvar);
		if (($tempvar>=$oldstart)&&($tempvar<=$oldend))
			{
			$tempvar=$tempvar+$offset;
			@words[$i]=$tempvar;
			}
	$i++;
	$line=join(" ",@words,"\n");
	}

}



	 if ($position >= 4)
	 {
			@words= split(/ /,$line);
			foreach $tempvar (@words)
			{
				chomp($tempvar);
				if (($tempvar>=$oldstart)&&($tempvar<=$oldend))
					{
				 	 $tempvar=$tempvar+$offset;
					}
				else
					{
					if(($tempvar>700)&&($tempvar<70000))
					 {
						print ERRORFILE "line requires inspection:";
						print ERRORFILE $count;
					 }
					}
			}
		 	$line= join(" ",@words,"\n");
	 }
}



print TEMPFILE $line;



$count++;

}
close(VNUMFILE);
close(TEMPFILE);
close(ERRORFILE);
print"\nAll done, file named 'areafile' created with all the changes.\n Check 
'error' for possible errors.\n";

