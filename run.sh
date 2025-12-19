#!/bin/bash
set -e

#config
DISK_IMG="hda.img"
DISK_SIZE_MB=64
EFI_BINARY="BOOTX64.EFI"
OVMF_CODE="boot/uefi/OVMF_CODE.4m.fd"

print_step() {
    echo -e "==> $1"
}

#create GPT disk image and ESP
create_disk_image() {
    print_step "creating disk image"
    dd if=/dev/zero of="$DISK_IMG" bs=1M count=$DISK_SIZE_MB status=none

    #partition table creation
    sgdisk --clear --new=1:2048:0 --typecode=1:EF00 --change-name=1:"EFI System" "$DISK_IMG"


    #calculate offset for mtools
    local PART_OFFSET=$((2048 * 512))

    #format partition as fat32
    mformat -F -i "$DISK_IMG"@@${PART_OFFSET} ::

    #setup directory structure and copy binaries
    mmd -i "$DISK_IMG"@@${PART_OFFSET} ::/EFI
    mmd -i "$DISK_IMG"@@${PART_OFFSET} ::/EFI/BOOT

    if [[ -f "$EFI_BINARY" ]]; then
        mcopy -i "$DISK_IMG"@@${PART_OFFSET} "$EFI_BINARY" ::/EFI/BOOT/BOOTX64.EFI
    else
        echo "error: $EFI_BINARY missing."
        exit 1
    fi

    [[ -f "boot/delboot.cfg" ]] && mcopy -i "$DISK_IMG"@@${PART_OFFSET} "boot/delboot.cfg" ::/EFI/BOOT/delboot.cfg
}

#execute qemu with ovmf
run_qemu() {
    print_step "launching qemu"
    local QEMU_ARGS=(
        -machine q35
        -cpu qemu64
        -m 256M
        -drive "if=pflash,format=raw,readonly=on,file=$OVMF_CODE"
        -drive "file=$DISK_IMG,format=raw"
        -net none
        -serial stdio
    )

    #handle writable variables if available
    if [[ -n "$OVMF_VARS" ]]; then
        [[ ! -f "ovmf_vars.fd" ]] && cp "$OVMF_VARS" "ovmf_vars.fd"
        QEMU_ARGS+=(-drive "if=pflash,format=raw,file=ovmf_vars.fd")
    fi

    qemu-system-x86_64 "${QEMU_ARGS[@]}"
}

main() {
    #dependency check
    for cmd in qemu-system-x86_64 sgdisk mformat mmd mcopy; do
        if ! command -v "$cmd" &>/dev/null  ; then
            echo "error: $cmd is not installed."
            exit 1
        fi
    done

    #compile if source is newer than binary
    make > /dev/null

    create_disk_image
    run_qemu
}

main "$@"
