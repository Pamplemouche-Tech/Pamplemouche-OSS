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

    // Variables pour la carte mémoire
    UINTN memory_map_size = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = (void*)0;
    UINTN map_key = 0;
    UINTN descriptor_size = 0;
    unsigned int descriptor_version = 0;

    // 1. Premier appel pour obtenir la taille requise dans `memory_map_size`
    SystemTable->BootServices->GetMemoryMap(
        &memory_map_size, 
        memory_map, 
        &map_key, 
        &descriptor_size, 
        &descriptor_version
    );

    // On ajoute une marge de sécurité au cas où l'allocation elle-même modifierait la carte mémoire
    memory_map_size += 2 * descriptor_size;

    // 2. Allocation du tampon en mémoire RAM via AllocatePool
    SystemTable->BootServices->AllocatePool(
        EfiLoaderData, 
        memory_map_size, 
        (void**)&memory_map
    );

    // 3. Second appel pour remplir le tampon alloué avec la vraie carte mémoire
    EFI_STATUS status = SystemTable->BootServices->GetMemoryMap(
        &memory_map_size, 
        memory_map, 
        &map_key, 
        &descriptor_size, 
        &descriptor_version
    );

    // Si le statut est EFI_SUCCESS (0), la carte mémoire est chargée en RAM !
    if (status == EFI_SUCCESS) {
        uint16_t success_message[] = {
            'C', 'a', 'r', 't', 'e', ' ', 'm', 'e', 'm', 'o', 'i', 'r', 'e', ' ', 'c', 'h', 'a', 'r', 'g', 'e', 'e', ' ', 'e', 'n', ' ', 'R', 'A', 'M', '.',
            '\r', '\n', 0
        };
        SystemTable->ConOut->OutputString(SystemTable->ConOut, success_message);
    }

    // On fige le processeur
    while(1) {
        __asm__ __volatile__("hlt");
    }

    return EFI_SUCCESS;
}
