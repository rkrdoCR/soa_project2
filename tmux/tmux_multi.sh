#!/bin/bash

TOP_REF=1
MIDDLE_1_REF=2
MIDDLE_2_REF=3
BOTTOM_REF=4
PANE_REF=2
NUMBER_OF_PANES=0



make_new_pane () {
    tmux split-window -v
    tmux send-keys "source $PROJECT2_SEMAPHORES_HOME/set_environment_variables" C-m
}

create_pane () {
    read -p "Enter how many panes do you want> " number_of_panes
    i=1
    NUMBER_OF_PANES=$number_of_panes
    while [ $i -lt $number_of_panes ]
    do
    
    make_new_pane

    i=$[$i+1]
    done
}

create_multi_pane () {
    if [ "$ARG_ONE" == "p" ]; then
        PANE_REF=2
    elif [ "$ARG_ONE" == "c" ]; then
        PANE_REF=5
    else
        PANE_REF=2
    fi
    read -e -p "Enter pane reference number> " -i "$PANE_REF" ref_pane
    PANE_REF=$ref_pane
    create_pane
}

launch_program () {
    i=0
    while [ $i -lt $NUMBER_OF_PANES ]
    do
    SUM=$[$PANE_REF+$i]
    tmux select-pane -t $SUM
    tmux send-keys "clear" C-m
    tmux send-keys "./$name_exe $id_mem $[$RANDOM%10 + 1]" C-m
    i=$[$i+1]
    done
}

########################################
## input from user
########################################

ARG_ONE=$1

if [ "$1" = "p" ]; then
    #echo "Positional parameter 1 contains something"
    read -e -p "Enter name of executable> " -i "producer_main" name_exe
elif [ "$1" = "c" ]; then
    #echo "Positional parameter 1 contains something"
    read -e -p "Enter name of executable> " -i "consumer_main" name_exe
else
    #echo "Positional parameter 1 is empty"
    read -e -p "Enter name of executable> " -i "producer_main" name_exe
fi

read -p "Enter ID of shared memory> " id_mem

#########################################



cd $PROJECT2_SEMAPHORES_HOME/build

create_multi_pane

launch_program
