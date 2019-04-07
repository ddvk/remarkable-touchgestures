TARGET=touchinjector

$(TARGET): main.c
	$(CC) -std=c99 -lm -o $@ main.c
clean:
	rm $(TARGET)
