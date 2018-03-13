#pragma once

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include "Bitmap.h"

/** @defgroup Graphics Graphics
 * @{
 * Funcoes responsaveis pela manipulacao grafica
 */

//declaring the buffer to be used

char *background_buffer; //Parte do triple buffer
char *mouse_buffer; //Parte do triple buffer

//V_GR STUFF

/**
 * @brief Initializa os graficos no modo especificado
 *
 * @param mode Modo utilizado para a inicializacao
 * @return Endereco VM do frame buffer
 */

void *vg_init(unsigned short mode);

/**
 * @brief Sai do modo grafico
 *
 * @return 1 se a operacao falhar, 0 se tiver sucesso
 */

int vg_exit(void);

/**
 * @brief Desenha um pixel numa determinada posicao
 *
 * @param x Coordenada x do pixel
 * @param y Coordenada y do pixel
 * @param color Cor do pixel
 * @return 1 se falhar, 0 se tiver sucesso
 */

int vg_set_pixel(unsigned short x, unsigned short y, unsigned short color);

/**
 * @brief Desenha um quadrado numa determinada posicao
 *
 * @param x Coordenada x inicial do quadrado
 * @param y Coordenada y inicial do quadrado
 * @param size Tamanho do quadrado
 * @param color Cor do pixel
 * @return 1 se falhar, 0 se tiver sucesso
 */

int vg_draw_square(unsigned int x, unsigned int y, unsigned short size, unsigned int color);

/**
 * @brief Desenha um sprite numa determinada posicao
 *
 * @param xpm endereco do primeiro char do array que contem a imagem xpm
 * @param y Coordenada y inicial do sprite
 * @return 1 se falhar, 0 se tiver sucesso
 */

int vg_draw_sprite(char *xpm[], unsigned short xi, unsigned short yi);

/**
 * @brief Inicializa os buffers graficos
 */

void initialize_buffers();

/**
 * @brief Liberta a memoria ocupada pelos buffers graficos
 */

void delete_buffers();

/**
 * @brief Copia o conteudo do buffer do background para o buffer do mouse
 */

void background_to_mouse_buffer();

/**
 * @brief Copia o conteudo do buffer do mouse para a VRAM
 */

void mouse_buffer_to_video_mem();

//VBE STUFF



typedef struct {
  /*  Mandatory information for all VBE revisions */
  uint16_t ModeAttributes; 	/**< @brief mode attributes */
  uint8_t WinAAttributes; 		/**< @brief window A attributes */
  uint8_t WinBAttributes; 		/**< @brief window B attributes */
  uint16_t WinGranularity; 	/**< @brief window granularity */
  uint16_t WinSize;		/**< @brief window size */
  uint16_t WinASegment;		/**< @brief window A start segment */
  uint16_t WinBSegment;		/**< @brief window B start segment */
  phys_bytes WinFuncPtr;	/**< @brief real mode/far pointer to window function */
  uint16_t BytesPerScanLine; 	/**< @brief bytes per scan line */

  /* Mandatory information for VBE 1.2 and above */

  uint16_t XResolution;      	/**< @brief horizontal resolution in pixels/characters */
  uint16_t YResolution;      	/**< @brief vertical resolution in pixels/characters */
  uint8_t XCharSize; 		/**< @brief character cell width in pixels */
  uint8_t YCharSize; 		/**< @brief character cell height in pixels */
  uint8_t NumberOfPlanes; 		/**< @brief number of memory planes */
  uint8_t BitsPerPixel; 		/**< @brief bits per pixel */
  uint8_t NumberOfBanks;		/**< @brief number of banks */
  uint8_t MemoryModel;		/**< @brief memory model type */
  uint8_t BankSize;		/**< @brief bank size in KB */
  uint8_t NumberOfImagePages;	/**< @brief number of images */
  uint8_t Reserved1;		/**< @brief reserved for page function */

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */

  uint8_t RedMaskSize;		/* size of direct color red mask in bits */
  uint8_t RedFieldPosition;	/* bit position of lsb of red mask */
  uint8_t GreenMaskSize;		/* size of direct color green mask in bits */
  uint8_t GreenFieldPosition;	/* bit position of lsb of green mask */
  uint8_t BlueMaskSize; 		/* size of direct color blue mask in bits */
  uint8_t BlueFieldPosition;	/* bit position of lsb of blue mask */
  uint8_t RsvdMaskSize;		/* size of direct color reserved mask in bits */
  uint8_t RsvdFieldPosition;	/* bit position of lsb of reserved mask */
  uint8_t DirectColorModeInfo;	/* direct color mode attributes */

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;       /**< @brief physical address for flat memory frame buffer */
  uint8_t Reserved2[4]; 		/**< @brief Reserved - always set to 0 */
  uint8_t Reserved3[2]; 		/**< @brief Reserved - always set to 0 */

  /* Mandatory information for VBE 3.0 and above */
  uint16_t LinBytesPerScanLine;    /* bytes per scan line for linear modes */
  uint8_t BnkNumberOfImagePages; 	/* number of images for banked modes */
  uint8_t LinNumberOfImagePages; 	/* number of images for linear modes */
  uint8_t LinRedMaskSize; 	        /* size of direct color red mask (linear modes) */
  uint8_t LinRedFieldPosition; 	/* bit position of lsb of red mask (linear modes) */
  uint8_t LinGreenMaskSize; 	/* size of direct color green mask (linear modes) */
  uint8_t LinGreenFieldPosition; /* bit position of lsb of green mask (linear  modes) */
  uint8_t LinBlueMaskSize; 	/* size of direct color blue mask (linear modes) */
  uint8_t LinBlueFieldPosition; 	/* bit position of lsb of blue mask (linear modes ) */
  uint8_t LinRsvdMaskSize; 	/* size of direct color reserved mask (linear modes) */
  uint8_t LinRsvdFieldPosition;	 /* bit position of lsb of reserved mask (linear modes) */
  uint32_t MaxPixelClock; 	         /* maximum pixel clock (in Hz) for graphics mode */
  uint8_t Reserved4[190]; 		 /* remainder of ModeInfoBlock */
} __attribute__((packed)) vbe_mode_info_t;


typedef struct {
	uint8_t VbeSignature[4];
	uint16_t VBeVersion;
	phys_bytes OemStringPtr;
	uint8_t Capabilities[4];
	phys_bytes VideoModePtr;
	uint16_t TotalMemory;
	uint16_t OemSoftwareRev;
	phys_bytes OemVendorNamePtr;
	phys_bytes OemProductNamePtr;
	phys_bytes OemProductRevPtr;
	uint8_t Reserved[222];
	uint8_t OemData[256];

} __attribute__((packed)) vbe_controller_info_t;

/**
 * @brief Disponibiliza a informacao do modo respetivo
 *
 * @param mode Modo a consultar
 * @param vmi_p Endereco virtual da VRAM
 * @return 1 se falhar, 0 se tiver sucesso
 */

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Modifica o modo grafico ativo
 *
 * @param mode Modo a consultar
 * @return 1 se falhar, 0 se tiver sucesso
 */

int vbe_set_mode(unsigned short mode);

/**
 * @brief Disponibiliza a informacao sobre o controlador grafico
 *
 * @param vci_p Apontador para a localizacao de memoria onde a informacao sera guardada
 * @return 1 se falhar, 0 se tiver sucesso
 */

int vbe_get_controller_info(vbe_controller_info_t *vci_p);


//LIBM STUFF


typedef struct {
  phys_bytes phys;	/**< @brief physical address */
  void *virtual;	/**< @brief virtual address */
  unsigned long size;   /**< @brief size of memory region */
} mmap_t;

void *lm_init(void);


void *lm_alloc(unsigned long size, mmap_t *map);


void lm_free(mmap_t *map);






