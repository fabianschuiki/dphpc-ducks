all: \
	er_20_30.bin \
	er_10_20.bin \
	er_100_200.bin \
	er_1000_2000.bin \
	er_10000_20000.bin \
	er_100000_200000.bin \
	er_1000000_2000000.bin

er_%.bin:
	../build/graphgen $(subst _, ,$*) $@
