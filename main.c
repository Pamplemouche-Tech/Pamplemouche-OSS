#include "uefi.h"

// Implémentation maison de memcpy requise par le compilateur pour les initialisations de tableaux
void *memcpy(void *dest, const void *src, UINTN n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    for (UINTN i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    // Chaîne UTF-16 terminée par un caractère nul (0)
    // \r\n est le retour à la ligne standard pour l'UEFI
    uint16_t message[] = {
        'C', 'e', 'r', 'v', 'e', 'a', 'u', ' ', 'e', 't', ' ', 
        'M', 'a', 'i', 'n', 's', ' ', 'o', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', 'n', 'e', 'l', 's', '.',
        '\r', '\n', 0
    };

    // On demande à l'UEFI d'afficher la chaîne à l'écran
    SystemTable->ConOut->OutputString(SystemTable->ConOut, message);

    // On fige le processeur dans une boucle basse consommation pour observer le résultat
    while(1) {
        __asm__ __volatile__("hlt");
    }

    return EFI_SUCCESS;
}
