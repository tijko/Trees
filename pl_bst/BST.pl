package Tree;

use strict;
use warnings;


sub insert_node;
sub retrieve_node;
sub dump_tree;

sub new {
    my $class = shift;
    my $self = { "root" => undef };
    bless $self, $class;
    return $self;
}

sub bst_menu {

    my $tree = new Tree();

    while (1) {

        print "Select option: [1=insert, 2=delete, 3=search, 4=tree_walk, 5=exit]\n";
        print "Enter Selection: ";

        my $choice = <>;
        chomp($choice);

        if ($choice =~ /\D/) {
            print "Invalid Selection!\n";
        } elsif ($choice == 1) {
            my $value = input_value();
            insert_node \$tree->{root}, undef, $value;
        } elsif ($choice == 2) {
            my $value = input_value();
            delete_node $tree, $value;
        } elsif ($choice == 3) {
            my $value = input_value();
            my $node = retrieve_node \$tree->{root}, $value;
            if (!$$node) {
                print "$value is not in Tree!\n";
            } else {
                print "Found $value!\n";
            }
        } elsif ($choice == 4) {
            dump_tree $tree->{root};
        } elsif ($choice == 5) {
            return;
        }
    }

    return;
}

sub insert_node {

    my ($tree, $parent, $value) = @_;

    if (!$$tree) {

        $$tree = new Tree();

        $$tree->{value} = $value;
        $$tree->{parent} = $parent;

        $$tree->{left} = undef;
        $$tree->{right} = undef;

        return;

    } elsif ($$tree->{value} >= $value) { 
        insert_node \$$tree->{left}, $$tree, $value;
    } else {
        insert_node \$$tree->{right}, $$tree, $value;
    }
}        
    
sub delete_node {
    return;
}

sub transplant {
    return;
}

sub min_tree {

    my ($tree) = @_;

    if (!$tree->{left}) {
        return $tree;
    }

    return min_tree($tree->{left});
}

sub retrieve_node {

    my ($tree, $value) = @_;

    if (!$$tree) {
        return $tree;
    } elsif ($$tree->{value} == $value) {
        return $tree;
    } elsif ($$tree->{value} > $value) {
        retrieve_node \$$tree->{left}, $value;
    } else {
        retrieve_node \$$tree->{right}, $value;
    }
}

sub dump_tree {

    my ($tree) = @_;    

    if ($tree) {
        if ($tree->{left} && $tree->{right}) {
            print "Node: $tree->{value} - Left: $tree->{left}->{value} ";
            print "Right: $tree->{right}->{value}\n";
        } elsif ($tree->{left} && !$tree->{right}) {
            print "Node: $tree->{value} - Left: $tree->{left}->{value} ";
            print "Right: <Null>\n";
        } elsif ($tree->{right} && !$tree->{left}) {
            print "Node: $tree->{value} - Left: <Null> ";
            print "Right: $tree->{right}->{value}\n";
        } elsif (!$tree->{right} && !$tree->{left} && !$tree->{parent}) {
            print "Node: $tree->{value}\n";
        }

        dump_tree $tree->{left};
        dump_tree $tree->{right};
    }

    return;
}

sub input_value {

    my $value;

    while (1) {
        print "Enter Value: ";
        $value = <>;
        chomp($value);
        if ($value =~ /\D/) {
            print "Invalid Value! (input integers only) ...\n";
        } else {
            last;
        }
    }

    return $value;
}

bst_menu();
