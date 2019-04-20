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
    read -p "Enter pane reference number> " ref_pane
    PANE_REF=$ref_pane
    create_pane
}

launch_program () {
    i=0
    while [ $i -lt $NUMBER_OF_PANES ]
    do
    SUM=$[$PANE_REF+$i]
    tmux select-pane -t $SUM
    tmux send-keys "./$name_exe $id_mem 2" C-m
    i=$[$i+1]
    done
}

cd $PROJECT2_SEMAPHORES_HOME/build

read -e -p "Enter name of executable> " name_exe

read -p "Enter ID of shared memory> " id_mem

create_multi_pane

launch_program


 
# tmux send-keys "clear" C-m
# tmux send-keys "echo top" C-m
# tmux send-keys "source $PROJECT2_SEMAPHORES_HOME/set_environment_variables" C-m
# tmux send-keys "cd $PROJECT2_SEMAPHORES_HOME/build" C-m
# tmux send-keys "clear" C-m