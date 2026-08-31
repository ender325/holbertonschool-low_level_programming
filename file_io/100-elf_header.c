#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

void check_elf(unsigned char *e_ident);
void print_magic_class_data(unsigned char *e_ident);
void print_osabi_abi(unsigned char *e_ident);
void print_type_entry(unsigned char *e_ident, unsigned int e_type,
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
 * print_magic_class_data - Prints magic, class, data, and version.
 * @e_ident: Pointer to ELF magic numbers array.
 */
void print_magic_class_data(unsigned char *e_ident)
{
	int i;

	printf("ELF Header:\n  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
		printf("%02x ", e_ident[i]);
	printf("\n");

	printf("  %-35s", "Class:");
	if (e_ident[EI_CLASS] == ELFCLASS32)
		printf("ELF32\n");
	else if (e_ident[EI_CLASS] == ELFCLASS64)
		printf("ELF64\n");
	else if (e_ident[EI_CLASS] == ELFCLASSNONE)
		printf("none\n");
	else
		printf("<unknown: %x>\n", e_ident[EI_CLASS]);

	printf("  %-35s", "Data:");
	if (e_ident[EI_DATA] == ELFDATA2LSB)
		printf("2's complement, little endian\n");
	else if (e_ident[EI_DATA] == ELFDATA2MSB)
		printf("2's complement, big endian\n");
	else if (e_ident[EI_DATA] == ELFDATANONE)
		printf("none\n");
	else
		printf("<unknown: %x>\n", e_ident[EI_DATA]);

	printf("  %-35s", "Version:");
	if (e_ident[EI_VERSION] == EV_CURRENT)
		printf("1 (current)\n");
	else
		printf("%d\n", e_ident[EI_VERSION]);
}

/**
 * print_osabi_abi - Prints OS/ABI and ABI version.
 * @e_ident: Pointer to ELF magic numbers array.
 */
void print_osabi_abi(unsigned char *e_ident)
{
	unsigned char osabi = e_ident[EI_OSABI];

	printf("  %-35s", "OS/ABI:");
	if (osabi == ELFOSABI_NONE)
		printf("UNIX - System V\n");
	else if (osabi == ELFOSABI_HPUX)
		printf("UNIX - HP-UX\n");
	else if (osabi == ELFOSABI_NETBSD)
		printf("UNIX - NetBSD\n");
	else if (osabi == ELFOSABI_GNU)
		printf("UNIX - GNU\n");
	else if (osabi == ELFOSABI_SOLARIS)
		printf("UNIX - Solaris\n");
	else if (osabi == ELFOSABI_AIX)
		printf("UNIX - AIX\n");
	else if (osabi == ELFOSABI_IRIX)
		printf("UNIX - IRIX\n");
	else if (osabi == ELFOSABI_FREEBSD)
		printf("UNIX - FreeBSD\n");
	else if (osabi == ELFOSABI_TRU64)
		printf("UNIX - TRU64\n");
	else if (osabi == ELFOSABI_MODESTO)
		printf("Novell - Modesto\n");
	else if (osabi == ELFOSABI_OPENBSD)
		printf("UNIX - OpenBSD\n");
	else if (osabi == ELFOSABI_ARM_AEABI)
		printf("ARM - EABI\n");
	else if (osabi == ELFOSABI_ARM)
		printf("ARM\n");
	else if (osabi == ELFOSABI_STANDALONE)
		printf("Standalone App\n");
	else
		printf("<unknown: %x>\n", osabi);

	printf("  %-35s%d\n", "ABI Version:", e_ident[EI_ABIVERSION]);
}

/**
 * print_type_entry - Prints file type and entry point address.
 * @e_ident: Pointer to ELF magic numbers array.
 * @e_type: ELF type.
 * @e_entry: Entry point address.
 */
void print_type_entry(unsigned char *e_ident, unsigned int e_type,
		      unsigned long e_entry)
{
	int i;
	unsigned long res = 0;
	unsigned long src = e_entry;

	if (e_ident[EI_DATA] == ELFDATA2MSB)
	{
		e_type = ((e_type >> 8) & 0xFF) | ((e_type & 0xFF) << 8);
		if (e_ident[EI_CLASS] == ELFCLASS32)
		{
			e_entry = ((e_entry & 0xFF000000) >> 24) |
				  ((e_entry & 0x00FF0000) >> 8) |
				  ((e_entry & 0x0000FF00) << 8) |
				  ((e_entry & 0x000000FF) << 24);
		}
		else
		{
			for (i = 0; i < 8; i++)
			{
				res = (res << 8) | (src & 0xFF);
				src >>= 8;
			}
			e_entry = res;
		}
	}

	printf("  %-35s", "Type:");
	if (e_type == ET_NONE)
		printf("NONE (None)\n");
	else if (e_type == ET_REL)
		printf("REL (Relocatable file)\n");
	else if (e_type == ET_EXEC)
		printf("EXEC (Executable file)\n");
	else if (e_type == ET_DYN)
		printf("DYN (Shared object file)\n");
	else if (e_type == ET_CORE)
		printf("CORE (Core file)\n");
	else
		printf("<unknown: %x>\n", e_type);

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
	print_magic_class_data(header.e_ident);
	print_osabi_abi(header.e_ident);

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

	print_type_entry(header.e_ident, e_type, e_entry);

	close(fd);
	return (0);
}
