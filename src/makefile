# Makefile
# *** Test ***

main : main.o controller.o model.o view_console.o view_gtkmm.o connect_db.o
	g++ -g -Wall -o app main.o controller.o model.o view_console.o view_gtkmm.o connect_db.o `pkg-config gtkmm-3.0 --cflags --libs` -l sqlite3

main.o : main.cpp controller.hpp
	g++ -g -Wall -c main.cpp `pkg-config gtkmm-3.0 --cflags --libs`

controller.o : controller.cpp controller.hpp view_console.hpp view_gtkmm.hpp model.hpp
	g++ -g -Wall -c controller.cpp `pkg-config gtkmm-3.0 --cflags --libs`

view_gtkmm.o : view_gtkmm.cpp view_gtkmm.hpp
	g++ -g -Wall -c view_gtkmm.cpp `pkg-config gtkmm-3.0 --cflags --libs`

view_console.o : view_console.cpp view_console.hpp
	g++ -g -Wall -c view_console.cpp	

model.o : model.cpp model.hpp connect_db.hpp
	g++ -g -Wall -c model.cpp

connect_db.o : connect_db.cpp connect_db.hpp
	g++ -g -Wall -c connect_db.cpp -l sqlite3	

clean :
	rm -f app main.o controller.o view_gtkmm.o view_console.o model.o connect_db.o