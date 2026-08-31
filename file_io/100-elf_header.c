#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

void check_elf(unsigned char *e_ident);
void print_magic_and_class(unsigned char *e_ident);
void print_data_and_version(unsigned char *e_ident);
void print_osabi_and_abi(unsigned char *e_ident);
void print_type_and_entry(unsigned char *e_ident, unsigned int e_type,
			  unsigned long e_entry);

/**
 * check_elf - Checks if a file is an ELF file.
 * @e_ident: Pointer to an array containing the ELF magic numbers.
 */
void check_elf(unsigned char *e_ident)
{
	if (e_ident[EI_MAG0] != ELFMAG0 ||
	    e_ident[EI_MAG1] != ELFMAG1 ||
	    e_ident[EI_MAG2] != ELFMAG2 ||
	    e_ident[EI_MAG3] != ELFMAG3)
	{
		dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
		exit(98);
	}
}

/**
 * print_magic_and_class - Prints magic numbers and class of ELF header.
 * @e_ident: Pointer to ELF magic numbers array.
 */
void print_magic_and_class(unsigned char *e_ident)
{
	int i;

	printf("ELF Header:\n  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", e_ident[i]);
		if (i == EI_NIDENT - 1)
			printf("\n");
		else
			printf(" ");
	}

	printf("  %-35s", "Class:");
	switch (e_ident[EI_CLASS])
	{
	case ELFCLASSNONE:
		printf("none\n");
		break;
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_CLASS]);
	}
}

/**
 * print_data_and_version - Prints data encoding and version of ELF header.
 * @e_ident: Pointer to ELF magic numbers array.
 */
void print_data_and_version(unsigned char *e_ident)
{
	printf("  %-35s", "Data:");
	switch (e_ident[EI_DATA])
	{
	case ELFDATANONE:
		printf("none\n");
		break;
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_DATA]);
	}

	printf("  %-35s", "Version:");
	switch (e_ident[EI_VERSION])
	{
	case EV_CURRENT:
		printf("1 (current)\n");
		break;
	default:
		printf("%d\n", e_ident[EI_VERSION]);
	}
}

/**
 * print_osabi_and_abi - Prints OS/ABI and ABI version of ELF header.
 * @e_ident: Pointer to ELF magic numbers array.
 */
void print_osabi_and_abi(unsigned char *e_ident)
{
	printf("  %-35s", "OS/ABI:");
	switch (e_ident[EI_OSABI])
	{
	case ELFOSABI_NONE:
		printf("UNIX - System V\n");
		break;
	case ELFOSABI_HPUX:
		printf("UNIX - HP-UX\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_GNU:
		printf("UNIX - GNU\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	case ELFOSABI_AIX:
		printf("UNIX - AIX\n");
		break;
	case ELFOSABI_IRIX:
		printf("UNIX - IRIX\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("UNIX - FreeBSD\n");
		break;
	case ELFOSABI_TRU64:
		printf("UNIX - TRU64\n");
		break;
	case ELFOSABI_MODESTO:
		printf("Novell - Modesto\n");
		break;
	case ELFOSABI_OPENBSD:
		printf("UNIX - OpenBSD\n");
		break;
	case ELFOSABI_ARM_AEABI:
		printf("ARM - EABI\n");
		break;
	case ELFOSABI_ARM:
		printf("ARM\n");
		break;
	case ELFOSABI_STANDALONE:
		printf("Standalone App\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_OSABI]);
	}

	printf("  %-35s%d\n", "ABI Version:", e_ident[EI_ABIVERSION]);
}

/**
 * print_type_and_entry - Prints file type and entry point address.
 * @e_ident: Pointer to ELF magic numbers array.
 * @e_type: ELF type.
 * @e_entry: Entry point address.
 */
void print_type_and_entry(unsigned char *e_ident, unsigned int e_type,
			  unsigned long e_entry)
{
	if (e_ident[EI_DATA] == ELFDATA2MSB)
		e_type >>= 8;

	printf("  %-35s", "Type:");
	switch (e_type)
	{
	case ET_NONE:
		printf("NONE (None)\n");
		break;
	case ET_REL:
		printf("REL (Relocatable file)\n");
		break;
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	case ET_CORE:
		printf("CORE (Core file)\n");
		break;
	default:
		printf("<unknown: %x>\n", e_type);
	}

	if (e_ident[EI_DATA] == ELFDATA2MSB)
	{
		if (e_ident[EI_CLASS] == ELFCLASS32)
			e_entry = ((e_entry << 24) & 0xFF000000) |
				  ((e_entry << 8) & 0x00FF0000) |
				  ((e_entry >> 8) & 0x0000FF00) |
				  ((e_entry >> 24) & 0x000000FF);
		else
			e_entry = ((e_entry << 56) & 0xFF00000000000000ULL) |
				  ((e_entry << 40) & 0x00FF000000000000ULL) |
				  ((e_entry << 24) & 0x0000FF0000000000ULL) |
				  ((e_entry << 8) & 0x000000FF00000000ULL) |
				  ((e_entry >> 8) & 0x00000000FF000000ULL) |
				  ((e_entry >> 24) & 0x0000000000FF0000ULL) |
				  ((e_entry >> 40) & 0x000000000000FF00ULL) |
				  ((e_entry >> 56) & 0x00000000000000FFULL);
	}

	printf("  %-35s0x%lx\n", "Entry point address:", e_entry);
}

/**
 * main - Displays ELF header information of an ELF file.
 * @argc: Number of arguments passed to the program.
 * @argv: Array of pointers to arguments.
 *
 * Return: 0 on success, exits with status 98 on failure.
 */
int main(int argc, char *argv[])
{
	int fd;
	ssize_t r;
	Elf64_Ehdr header;
	Elf32_Ehdr *h32;
	unsigned int e_type;
	unsigned long e_entry;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: elf_header elf_filename\n");
		exit(98);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}

	r = read(fd, &header, sizeof(Elf64_Ehdr));
	if (r < (ssize_t)sizeof(Elf32_Ehdr))
	{
		dprintf(STDERR_FILENO, "Error: File too short %s\n", argv[1]);
		close(fd);
		exit(98);
	}

	check_elf(header.e_ident);
	print_magic_and_class(header.e_ident);
	print_data_and_version(header.e_ident);
	print_osabi_and_abi(header.e_ident);

	if (header.e_ident[EI_CLASS] == ELFCLASS32)
	{
		h32 = (Elf32_Ehdr *)&header;
		e_type = h32->e_type;
		e_entry = h32->e_entry;
	}
	else
	{
		e_type = header.e_type;
		e_entry = header.e_entry;
	}

	print_type_and_entry(header.e_ident, e_type, e_entry);

	close(fd);
	return (0);
}
