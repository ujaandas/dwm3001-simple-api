CC = gcc

uwb:
	gcc -I include src/main.c src/transport/tty.c src/board/commands.c src/packet/controlPacket.c src/packet/identifiers/gid.c src/packet/identifiers/oid.c src/packet/fields/mt.c src/packet/fields/pbf.c src/packet/identifiers/status.c -o main