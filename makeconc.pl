#! /usr/bin/perl
################################################################################
#
# File:         mkconc.pl
# RCS:          $Header: /home/matthew/cvs/bible-kjv-4.10/makeconc.pl,v 2.0 2003/01/08 15:29:52 matthew Exp $
# Description:  make Bible concordance: translation of Chip Chapin's ksh script
# Author:       Chris Eich, SRSD
# Created:      Wed Dec 23 11:00:18 1992
# Modified:     Wed Dec 23 15:49:23 1992 (Chip Chapin) chip@hpclbis
# Language:     perl
# Status:       Experimental (Do Not Distribute)
#
################################################################################
#
# Revisions:
#
# Wed Dec 23 15:19:45 1992 (Chip Chapin) chip@hpclbis
#  Received from Chris Eich, replaces "makeconcordance" script.
#  Made use of stopwords conditional.
###############################################################################

# Putting . on PATH ensures that the bible program will be found.
$ENV{'PATH'} =~ s/^:*/.:/;

$PROG = 'bible';

# Read a list of stop words, if any, one per line.

if (open(STOP, "$ARGV[0]")) {
    print "Excluding stopwords ($ARGV[0]) from concordance.\n";
    while (<STOP>) {
	# Ignore comments, mark stop word if one is found.
	$stopword{$&}++ if !/^#/ && /[a-z]+/;
    }
    close(STOP);
} else {
    print "All words will be included in concordance (no stopwords).\n";
}

# Generate plain text file, one "record" (e.g. bible verse) per line.
# Fill %lines and $count tables, which are keyed by words.

open(BIBLE, "$PROG -f gen1:1-rev99:99 |");
while (<BIBLE>) {
    s/^\S+\s+//;	# Cut off the record reference that starts each line.
    tr/A-Z/a-z/;	# Downcase.
    tr/a-z/ /c;		# Turn non-alpha into space.
    %seenonthisline = ();
    for $word (split(' ')) {
	next if $stopword{$word};
	$count{$word}++;	# Move below next line to count per-line.
	next if $seenonthisline{$word}++;
	$lines{$word} .= " " . $.;
    }
}
close(BIBLE);

# Create raw concordance, listing the lines where each word occurs.

open(RAWCONC, "> $PROG.rawconcordance") || die "$PROG.rawconcordance: $!\n";
for $word (sort keys %lines) {
    print RAWCONC $word, $lines{$word}, "\n";
}
close(RAWCONC);

# Also create a wordcounts file, which gives the number of lines in
# which each word occurs.  Note that we ARE counting cases where the
# same word is used several times in the same record.  See the comment
# above for "$count{$word}++" to change this to per-record.

open(COUNTS, "| sort -nrk 2 > $PROG.wordcounts");
while (($word, $count) = each %count) {
    print COUNTS $word, "\t", $count, "\n";
}
close(COUNTS);

__END__

# Next ... create a binary form of the raw concordance.
# This is handled by "makeconcfile", a program invoked from the
# BRS makefile.

# so we're all done now.

# Interesting statistic: 89198 chars in all the words in the Bible,
#			 617371 word-verse occurrances
# from...
#	awk '{chars += length($1); counts += $2}
#		END {print "chars=" chars " counts=" counts}' bible.wordcounts

# end

###############################################################################
# Gnu Emacs variables...
#
#   Local Variables:
#   mode:   	    	    	        perl
#   eval:   	    	    	        (auto-fill-mode 0)
#   default-header-comment-character:	?#
#   header-prefix:			"#! /usr/bin/perl"
#   header-suffix:			"#"
#   header-comment-character:		?#
#   end:
