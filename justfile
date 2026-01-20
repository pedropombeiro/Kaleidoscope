root_dir := justfile_directory()

export KALEIDOSCOPE_DIR := root_dir

flash:
    make -C examples/Devices/Keyboardio/Model100 flash
