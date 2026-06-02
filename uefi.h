#ifndef UEFI_H
#define UEFI_H

// Types fondamentaux imposés par la spécification UEFI
typedef unsigned short uint16_t;
typedef unsigned long long UINTN;
typedef void *EFI_HANDLE;
typedef long long EFI_STATUS;

// Codes de statut standard
#define EFI_SUCCESS 0

// Types d'allocation pour la mémoire
typedef enum {
    EfiMaxAllocateType
} EFI_ALLOCATE_TYPE;

// Types de mémoire de l'UEFI (on utilise LoaderData pour notre carte mémoire)
typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData, // Mémoire de données pour le bootloader/noyau
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

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
    // Fonction pour récupérer la carte de la mémoire RAM
    EFI_STATUS (*GetMemoryMap)(
        UINTN *MemoryMapSize,
        EFI_MEMORY_DESCRIPTOR *MemoryMap,
        UINTN *MapKey,
        UINTN *DescriptorSize,
        unsigned int *DescriptorVersion
    );
    // Nouvelle fonction : Allouer un espace mémoire dynamique (comme malloc)
    EFI_STATUS (*AllocatePool)(
        EFI_MEMORY_TYPE PoolType,
        UINTN Size,
        void **Buffer
    );
} EFI_BOOT_SERVICES;

// La table système principale
typedef struct {
    char Header[24];
    void *ConIn;
    void *ConInTo;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    void *StandardError;
    void *In;
    EFI_BOOT_SERVICES *BootServices;
} EFI_SYSTEM_TABLE;

#endif // UEFI_H
