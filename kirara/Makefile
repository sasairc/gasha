#        ,
#       ; `
#      ,|: |    ___
#     ; ``'  '       `
#     |: ||'  |_|_|_\ `
#     |`'||'  | O  O ||   % gcc kirara.c ../src/gasha.c -I ../src -o kirara
#     =二二||_|   .  ﾉﾉ
#     '~.~.~.~／ | | `
#  ＜￣＼迎春|\\\\:| |:/|
#   ／"(／~＼|:\\\\| |//|
#   レ'| |。|@]二   二|
#    ====(二|」//| |\\\\|
#    |_|_|_||、/:| |:`ﾉ
#             ￣￣￣￣

TARGET	= kirara
CC	= cc
RM	= rm
TARGET	= kirara
LIBS	= -lgasha
#DEFS	= -DVERBOSE -DPICKUP_KMB

all: $(TARGET)

kirara: kirara.c
	$(CC) $(LIBS) $^ -o $@ $(DEFS)

clean:
	-$(RM) -f $(TARGET)

.PHONY: all clean
