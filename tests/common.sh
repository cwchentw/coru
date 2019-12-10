export _assert=0;
assert ()
{
    local condition=$1;

    if [ -z "$condition" ];
    then
        "No valid condition" >&2;
        exit 1;
    fi

    shift;

    if [ "$condition" -ne 0 ];
    then
        echo $@ >&2;
        _assert=1;
    fi
}

assertAll ()
{
    if [ $_assert -ne 0 ];
    then
        exit 1;
    fi
    
    exit 0;
}