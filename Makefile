TARGET=bin/touchinjector

$(TARGET): main.c 
	@mkdir -p $(@D)
	$(CC) -std=c99 -lm -o $@ main.c
clean:
	rm $(TARGET)
