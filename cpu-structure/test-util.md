### Shell Functions For Testing

##### String Segmentation
```shell
core_list="0-7,8-15"
array=(${core_list//,/ })

for core in ${array[@]};
do
    echo $core
done
```

##### Run Multiple Instances
```shell
run_m_insts()
{
    insts_num=${1}  # instance number
    cmd=${2}        # instance
    for var in $(seq 1 ${insts_num});
    do
        echo "No.${var} —— ${cmd}"
        eval "${cmd} >> output.${var} &"
    done
}
```

##### Listening Background Process
```shell
wait_()
{
    prc_name=${1}  # waiting process
    echo "Listening...${prc_name}"
    # for var in {0..100000}  ## For Loop
    while true
    do
        alives=`ps -ef | grep "${prc_name}" | grep -v grep | wc -l`
        if [ ${alives} -ne 0 ]
        then
            sleep_time="0.01"  # 10ms
            sleep ${sleep_time}  # sleep to wait
        else
            break
        fi
    done
}
```

##### Shell Tuple
```shell
a_tuple="a b c d"  # tuple is separated by blankspace.
tuple=(${a_tuple})

for item in ${tuple[@]};
do
    echo $item
done
```

##### Shell Tuple with Index
```shell
a_tuple="a b c d"  # tuple is separated by blankspace.
tuple=(${a_tuple})

for idx in ${!tuple[@]};
do
    echo $idx ${tuple[$idx]}
done
```
