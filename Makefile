all: dir creator producer consumer

dir:
	mkdir -p build
	cd build

creator: dir 
	gcc -o build/creator_main creator/creator_main.c data_structures/shared_memory.c data_structures/circular_buffer.c data_structures/message.c utilities/time_util.c  utilities/check_mod.c   -Iutilities -Idata_structures

producer: dir
	gcc -o build/producer_main producer/producer_main.c data_structures/shared_memory.c data_structures/circular_buffer.c utilities/time_util.c utilities/exp_dist.c -Iutilities -Idata_structures -lm

consumer: dir
	gcc -o build/consumer_main consumer/consumer_main.c data_structures/shared_memory.c data_structures/circular_buffer.c utilities/time_util.c utilities/exp_dist.c -Iutilities -Idata_structures -lm