#!/usr/bin/perl -w
# 06-27-2007 @start_frame=(6530, 4887, 6134, 6535, 6475);
# 07-14-2007 
@start_frame=(2777,2805,2712,2802,2758);
$num_frames=6000;
$basedir="/media/disk/2007-07-14";
for($i=3; $i<=3; $i++){
	$thisbasedir=$basedir."/camera$i";
	mkdir($thisbasedir."/img");
	mkdir($thisbasedir."/fg");
	mkdir($thisbasedir."/pfg");
	my $syscall = "./extractframes $thisbasedir/dvgrab-001.avi ".$start_frame[$i-1]." $num_frames $thisbasedir";
	print $syscall."\n";
	system($syscall);
}
