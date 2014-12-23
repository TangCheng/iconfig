#!/bin/sed -f

#Remove comment
:l1
/\/\*/,/\*\// {
    /\*\//! {
        $! {
            N
            b l1
        }
    }
    s/\/\*.*\*\///g
}

#Remove some "INSERT INTO" sentense
:l2
/INSERT[[:blank:]]+INTO[^(]+[(]/,/[)][[:blank:]];/ {
    /[)][[:blank:]]*;/! {
        $! {
            N
            b l2
        }
    }
    #Remove the base_info read-only record insertion sentense
    s/INSERT[[:blank:]]+INTO[[:blank:]]+base_info[[:blank:]]*[(].*0[[:blank:]]*[)][[:blank:]]*;//g
    #Remove the network record insertion sentense
    s/INSERT[[:blank:]]+INTO[[:blank:]]+network[^(]*[(].*[[:blank:]]*[)][[:blank:]]*;//g
    #Remove the szyc record insertion sentense
    s/INSERT[[:blank:]]+INTO[[:blank:]]+szyc[[:blank:]]*[(].*[[:blank:]]*[)][[:blank:]]*;//g
}

# use "REPLACE INTO" instead of "INSERT INTO"
s/INSERT[[:blank:]]+INTO/REPLACE INTO/g
