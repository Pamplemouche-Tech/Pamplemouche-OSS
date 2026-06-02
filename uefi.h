#ifndef UEFI_H
#define UEFI_H

// Types fondamentaux imposés par la spécification UEFI
typedef unsigned short uint16_t;
typedef unsigned long long UINTN;
typedef void *EFI_HANDLE;
typedef long long EFI_STATUS;

// Codes de statut standard
#define EFI_SUCCESS 0

// Déclaration anticipée du protocole de sortie texte
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Définition du pointeur de fonction pour l'affichage de chaînes UTF-16
typedef EFI_STATUS (*EFI_TEXT_STRING)(
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, 
    uint16_t *String
);

// Protocole de sortie texte de l'UEFI (Console de sortie)
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void *Reset;
    EFI_TEXT_STRING OutputString;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Structure décrivant un descripteur de mémoire UEFI
typedef struct {
    unsigned int Type;
    unsigned int Padding;
    unsigned long long PhysicalStart;
    unsigned long long VirtualStart;
    unsigned long long NumberOfPages;
    unsigned long long Attribute;
} EFI_MEMORY_DESCRIPTOR;

// Déclaration des Boot Services (les services disponibles avant de lancer le noyau)
typedef struct {
    char Header[24];
    void *RaiseTPL;
    void *RestoreTPL;
    void *AllocatePages;
    void *FreePages;
    // Fonction critique pour récupérer la carte de la mémoire RAM
    EFI_STATUS (*GetMemoryMap)(
        UINTN *MemoryMapSize,
        EFI_MEMORY_DESCRIPTOR *MemoryMap,
        UINTN *MapKey,
        UINTN *DescriptorSize,
        unsigned int *DescriptorVersion
    );
    // ... Le reste des Boot Services est ignoré pour le moment
} EFI_BOOT_SERVICES;

// La table système principale (fournie par le firmware au démarrage)
typedef struct {
    char Header[24];
    void *ConIn;
    void *ConInTo;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    void *StandardError;
    void *In;
    EFI_BOOT_SERVICES *BootServices; // Ajout de l'accès aux Boot Services
} EFI_SYSTEM_TABLE;

#endif // UEFI_H
