CC      = gcc
#don't know what stabs do
PROF    = -O -g
C_FLAGS =  -Wall $(PROF) $(NOCRYPT)
L_FLAGS =  $(PROF)
LIBS	= -lcrypt 
O_FILES = ac.o ae.o ai.o am.o ao.o aw.o \
          alias.o ban.o comm.o const.o db.o db2.o effects.o fight.o flags.o \
	  handler.o healer.o interp.o iprog.o lookup.o magic.o \
	  magic2.o mprog.o recycle.o save.o skills.o special.o tables.o \
	  update.o quest.o class.o board.o magic3.o who_list.o mob_prog.o \
	  olc.o olc_act.o olc_save.o bit.o mem.o string.o mob_cmds.o \
	  wizutil.o objdump.o trap.o map.o random.o clan.o olc_mpcode.o 

fate: $(O_FILES)
	rm -f fate
	$(CC) $(L_FLAGS) -o fate $(O_FILES) $(LIBS)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
