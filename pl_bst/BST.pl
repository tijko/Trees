use strict;
use warnings;


package Tree;

sub create_branch {
    my $class = shift;
    my $self = {};
    bless $self, $class;
    return $self;
}

sub bst_menu {
    my $tree;
    while (1) {
        print "Select option: [1=insert, 2=delete, 3=search, 4=tree_walk, 5=exit]\n";
        print "Enter Selection: ";
        my $choice = <>;
        chomp($choice);
        if ($choice == 1) {
            my $value = input_value();
            $tree = insert($tree, $value);
        } elsif ($choice == 2) {
            my $value = input_value();
            $tree = remove($tree, $value);
        } elsif ($choice == 3) {
            my $value = input_value();
            search($tree, $value);
        } elsif ($choice == 4) {
            dump_tree($tree);
        } elsif ($choice == 5) {
            return;
        } else {
            print "Invalid Selection!\n";
        }
    }
    return;
}

sub insert {
    my ($value, $tree) = @_;
    my $parent;
    my $branch = $tree;
    while (1) {
        $parent = $branch;
        if (!$branch) {
            $branch = create_branch Tree();
            $branch->{value} = $value;
            $branch->{parent} = $parent;
            if ($parent && $parent->{value} >= $value) {
                $parent->{left} = $branch;
            } elsif ($parent && $parent->{value} < $value) {
                $parent->{right} = $branch;
            }
            last;
        } elsif ($branch->{value} >= $value) {
            $branch = $branch->{left};
        } else {
            $branch = $branch->{right};
        }
    }
    return $tree;
}

sub remove {
    my ($value, $tree) = @_;
    return $tree;
}

sub search {
    my ($value, $tree) = @_;
    if (!$tree) {
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
    my $tree = @_;
    if ($tree) {
        print "$tree->{value}\n";
        dump_tree($tree->{left});
        dump_tree($tree->{right});
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
