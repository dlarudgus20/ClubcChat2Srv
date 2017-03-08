all:
	mkdir -p build
	make -C ClubcChat2Srv

clean:
	make clean -C ClubcChat2Srv
	-rm build/*

