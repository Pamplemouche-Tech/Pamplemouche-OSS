#include "uefi.h"

// Implémentation maison de memcpy requise par le compilateur
void *memcpy(void *dest, const void *src, UINTN n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    for (UINTN i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Message de démarrage
    uint16_t message[] = {
        'C', 'e', 'r', 'v', 'e', 'a', 'u', ' ', 'e', 't', ' ', 
        'M', 'a', 'i', 'n', 's', ' ', 'o', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', 'n', 'e', 'l', 's', '.',
        '\r', '\n', 0
    };
    SystemTable->ConOut->OutputString(SystemTable->ConOut, message);

    // Variables requises pour stocker la carte mémoire
    UINTN memory_map_size = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = (void*)0; // Tampon nul au départ pour demander la taille requise
    UINTN map_key = 0;
    UINTN descriptor_size = 0;
    unsigned int descriptor_version = 0;

    // Premier appel à GetMemoryMap avec un tampon de 0 pour obtenir la taille réelle nécessaire dans `memory_map_size`
    SystemTable->BootServices->GetMemoryMap(
        &memory_map_size, 
        memory_map, 
        &map_key, 
        &descriptor_size, 
        &descriptor_version
    );

    // Message pour confirmer que l'analyse de la mémoire a commencé
    uint16_t mem_message[] = {
        'C', 'a', 'r', 't', 'e', ' ', 'm', 'e', 'm', 'o', 'i', 'r', 'e', ' ', 'a', 'n', 'a', 'l', 'y', 's', 'e', 'e', '.',
        '\r', '\n', 0
    };
    SystemTable->ConOut->OutputString(SystemTable->ConOut, mem_message);

    // On fige le processeur
    while(1) {
        __asm__ __volatile__("hlt");
    }

    return EFI_SUCCESS;
}
