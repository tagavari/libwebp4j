#Versions
LIBRARY_VERSION := 1.0.0
WEBP_VERSION := 1.2.2
JAVA_TARGET := 8

#Java compiler
JC := javac
JA := jar

#OS-specific values
ifeq ($(OS),Windows_NT)
	TARGET_OS := windows
	SHARED_LIB_EXT := dll

	WEBP_URL := https://storage.googleapis.com/downloads.webmproject.org/releases/webp/libwebp-$(WEBP_VERSION)-windows-x64.zip
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		TARGET_OS := linux
		SHARED_LIB_EXT := so

		WEBP_URL := https://storage.googleapis.com/downloads.webmproject.org/releases/webp/libwebp-$(WEBP_VERSION)-linux-x86-64.tar.gz
	else ifeq ($(UNAME_S),Darwin)
		TARGET_OS := mac
		SHARED_LIB_EXT := dylib

		UNAME_M := $(shell uname -m)
		ifeq ($(UNAME_M),x86_64)
			WEBP_URL := https://storage.googleapis.com/downloads.webmproject.org/releases/webp/libwebp-$(WEBP_VERSION)-mac-x86-64.tar.gz
		else
			WEBP_URL := https://storage.googleapis.com/downloads.webmproject.org/releases/webp/libwebp-$(WEBP_VERSION)-mac-arm64.tar.gz
		endif
	endif
endif

#Common directories
BUILD_DIR := build
SRC_DIR := src

PACKAGE_PATH = me/tagavari/libwebp4j
TARGET_C_LIB := $(BUILD_DIR)/$(PACKAGE_PATH)/libwebp4j.$(SHARED_LIB_EXT)
TARGET_JAVA_LIB := $(BUILD_DIR)/libwebp4j.jar

LIBWEBP_DIR := $(BUILD_DIR)/libwebp-$(WEBP_VERSION)

#C sources
SRC_DIR_C := $(SRC_DIR)/c
SRC_LIST_C := $(shell find $(SRC_DIR_C) -name '*.c')
OBJ_LIST_C := $(patsubst $(SRC_DIR_C)/%.c,$(BUILD_DIR)/%.o,$(SRC_LIST_C)) # ./src/c/file.c > ./build/file.o

#Java sources
SRC_DIR_JAVA := $(SRC_DIR)/java
SRC_LIST_JAVA := $(shell find $(SRC_DIR_JAVA) -name '*.java')
OBJ_LIST_JAVA := $(patsubst $(SRC_DIR_JAVA)/%.java,$(BUILD_DIR)/%.class,$(SRC_LIST_JAVA)) # ./src/java/com/package/file.java > ./build/com/package/file.class
GENERATED_HEADER_DIR := $(BUILD_DIR)/java_include

#Compiler flags
CPPFLAGS := -I$(SRC_DIR_C) \
	-I$(GENERATED_HEADER_DIR) \
	$(addprefix -I,$(shell find $(JAVA_HOME)/include -type d)) \
	-I$(LIBWEBP_DIR)/include
LDFLAGS := -L$(LIBWEBP_DIR)/lib -lwebp
JCFLAGS := --release $(JAVA_TARGET) -parameters

#Shorthand compile jar library
all: $(TARGET_JAVA_LIB)

#Fetch libwebp builds from Google's servers
$(LIBWEBP_DIR):
	mkdir $(LIBWEBP_DIR) && curl -s $(WEBP_URL) | tar -xf - -C $(LIBWEBP_DIR) --strip-components 1

#Compile object files to library
$(TARGET_C_LIB): $(OBJ_LIST_C)
	$(CC) $(OBJ_LIST_C) -o $@ $(LDFLAGS) -shared

#Compile .c to .o
$(BUILD_DIR)/%.o: $(SRC_DIR_C)/%.c $(LIBWEBP_DIR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

#Compile class files to jar
#$(JA) --create --file $(TARGET_JAVA_LIB) $(addprefix -C $(BUILD_DIR) ,$(patsubst $(BUILD_DIR)/%,%,$(shell find $(BUILD_DIR) -name '*.class')) $(patsubst $(BUILD_DIR)/%,%,$(TARGET_C_LIB)))
$(TARGET_JAVA_LIB): $(OBJ_LIST_JAVA) $(TARGET_C_LIB)
	$(JA) --create --file $(TARGET_JAVA_LIB) -C $(BUILD_DIR) /$(PACKAGE_PATH)

#Compile .java to .class and generate native headers
$(BUILD_DIR)/%.class: $(SRC_DIR_JAVA)/%.java
	@mkdir -p $(BUILD_DIR)
	@rm -f $(GENERATED_HEADER_DIR)/$(subst /,_,$*).h
	$(JC) -d $(BUILD_DIR) -cp $(SRC_DIR_JAVA) -h $(GENERATED_HEADER_DIR) $(JCFLAGS) $<

#Install library to local Maven repository
install: $(TARGET_JAVA_LIB)
	mvn install:install-file \
		-Dfile=$(TARGET_JAVA_LIB) \
		-DgroupId=me.tagavari \
		-DartifactId=libwebp4j \
		-Dversion=$(LIBRARY_VERSION) \
		-Dpackaging=jar \
		-DgeneratePom=true

clean:
	rm -r $(BUILD_DIR)

.PHONY: all clean
