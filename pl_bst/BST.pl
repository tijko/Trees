use strict;
use warnings;


package Tree;

sub new {
    my $class = shift;
    my $self = {};
    bless $self, $class;
    return $self;
}

sub insert;
sub dump_tree;

sub bst_menu {

    my $tree = new Tree();
    $tree->{root} = undef;

    while (1) {

        print "Select option: [1=insert, 2=delete, 3=search, 4=tree_walk, 5=exit]\n";
        print "Enter Selection: ";

        my $choice = <>;
        chomp($choice);

        if ($choice =~ /\D/) {
            print "Invalid Selection!\n";
        } elsif ($choice == 1) {
            my $value = input_value();
            insert \$tree->{root}, undef, $value;
        } elsif ($choice == 2) {
            my $value = input_value();
            $tree = remove($tree, $value);
        } elsif ($choice == 3) {
            my $value = input_value();
            search($tree, $value);
        } elsif ($choice == 4) {
            dump_tree $tree->{root};
        } elsif ($choice == 5) {
            return;
        }
    }

    return;
}

sub insert {

    my ($tree, $parent, $value) = @_;

    if (!$$tree) {

        $$tree = new Tree();
        $$tree->{value} = $value;

        $$tree->{parent} = $parent;
        $$tree->{left} = undef;
        $$tree->{right} = undef;

        return;

    } elsif ($$tree->{value} >= $value) { 
        insert \$$tree->{left}, $$tree, $value;
    } else {
        insert \$$tree->{right}, $$tree, $value;
    }
}        
    
sub remove {
    my ($tree, $value) = @_;
    my $branch = $tree;
    while ($branch->{value}) {
        if ($branch->{value} == $value) {
            last;
        } elsif ($branch->{value} >= $value) {
            $branch = $branch->{left};
        } else {
            $branch = $branch->{right};
        }
    }
    if (!$branch->{value}) {
        print "$value is NOT in tree!\n";
        return $tree;
    } else {
        if (!$branch->{left}->{value}) {
            transplant($tree, $branch, $branch->{right});
        } elsif (!$branch->{right}->{value}) {
            transplant($tree, $branch, $branch->{left});
        } else {
            my $splice = min_tree($branch->{right});
            if ($splice->{parent} != $branch) {
                transplant($tree, $splice, $splice->{right});
                $splice->{right} = $branch->{right};
                $splice->{right}->{parent} = $splice;
            }
            $tree = transplant($tree, $branch, $splice);
#            transplant($tree, $branch, $splice);
            $splice->{left} = $branch->{left};
            $splice->{left}->{parent} = $splice;
        }
    }
    return $tree;
}

sub transplant {
    my ($tree, $rm_branch, $branch) = @_;
    if (!$rm_branch->{parent}->{value}) {
        $branch->{parent} = undef;
        $tree = $branch;
        return $tree;
    } elsif ($rm_branch == $rm_branch->{parent}->{left}) {
        $rm_branch->{parent}->{left} = $branch;
    } else {
        $rm_branch->{parent}->{right} = $branch;
    }
    if ($branch->{value}) {
        $branch->{parent} = $rm_branch->{parent};
    }
    return $tree;
}

sub min_tree {
    my ($tree) = @_;
    if (!$tree->{left}->{value}) {
        return $tree;
    }
    return min_tree($tree->{left});
}

sub search {
    my ($tree, $value) = @_;
    if (!$tree->{value}) {
        print "$value is not in Tree!\n";
    } elsif ($tree->{value} == $value) {
        print "Found $value!\n";
    } elsif ($tree->{value} gt $value) {
        search($tree->{left}, $value);
    } else {
        search($tree->{right}, $value);
    }
}

sub dump_tree {

    my ($tree) = @_;    

    if ($tree->{value}) {
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
            print "Invalid Value only digits...\n";
        } else {
            last;
        }
    }

    return $value;
}

bst_menu();

