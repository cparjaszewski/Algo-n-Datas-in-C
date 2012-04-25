#!/usr/bin/perl -wl

$n = "01";
for $p (1..5) {
	for (1..5) {
		open(F, ">duze$n.in");
		select(F);
		print "5000 $p 999983";
		for (1..$p) {
			$x = sprintf("%d%d%d\n%d%d%d\n%d%d%d", 
				int(rand()*2), int(rand()*2), int(rand()*2),
				int(rand()*2), int(rand()*2), int(rand()*2),
				int(rand()*2), int(rand()*2), int(rand()*2));
			$x =~ s/0/./g;
			$x =~ s/1/x/g;
			print $x;
		}
		$n++;
	}
}
