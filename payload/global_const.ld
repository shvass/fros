SECTIONS {
    .ctors :
    {
        __ctors_start__ = .;
        KEEP(*(.ctors))
        __ctors_end__ = .;
    }

    .dtors :
    {
        __dtors_start__ = .;
        KEEP(*(.dtors))
        __dtors_end__ = .;
    }
}
