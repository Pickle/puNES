/*
 *  Copyright (C) 2010-2016 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <string.h>
#include "mappers.h"
#include "info.h"
#include "mem_map.h"
#include "irqA12.h"
#include "save_slot.h"

static void INLINE ks7037_update(void);

BYTE *ks7037_prg_7000;
BYTE *ks7037_prg_B000;

void map_init_KS7037(void) {
	EXTCL_AFTER_MAPPER_INIT(KS7037);
	EXTCL_CPU_WR_MEM(KS7037);
	EXTCL_CPU_RD_MEM(KS7037);
	EXTCL_SAVE_MAPPER(KS7037);
	mapper.internal_struct[0] = (BYTE *) &ks7037;
	mapper.internal_struct_size[0] = sizeof(ks7037);

	memset(&ks7037, 0x00, sizeof(ks7037));

	info.prg.ram.banks_8k_plus = 1;

	info.mapper.extend_rd = TRUE;
	info.mapper.extend_wr = TRUE;
	info.mapper.ram_plus_op_controlled_by_mapper = TRUE;
}
void extcl_after_mapper_init_KS7037(void) {
	// posso farlo solo dopo il map_prg_ram_init();
	ks7037_update();
}
void extcl_cpu_wr_mem_KS7037(WORD address, BYTE value) {
	if (address < 0x6000) {
		return;
	}
	if ((address >= 0x6000) && (address <= 0x6FFF)) {
		prg.ram_plus_8k[address & 0x0FFF] = value;
		return;
	}
	if ((address >= 0x7000) && (address <= 0x7FFF)) {
		return;
	}
	if ((address >= 0xA000) && (address <= 0xBFFF)) {
		if (address >= 0xB000) {
			ks7037_prg_B000[address & 0x0FFF] = value;
		}
		return;
	}

	switch (address & 0xE001) {
		case 0x8000:
			ks7037.ind = value & 0x07;
			break;
		case 0x8001:
			ks7037.reg[ks7037.ind] = value;
			ks7037_update();
			break;
	}
}
BYTE extcl_cpu_rd_mem_KS7037(WORD address, BYTE openbus, BYTE before) {
	if ((address >= 0x6000) && (address <= 0x6FFF)) {
		return (prg.ram_plus_8k[address & 0x0FFF]);
	}
	if ((address >= 0x7000) && (address <= 0x7FFF)) {
		return (ks7037_prg_7000[address & 0x0FFF]);
	}
	if ((address >= 0xA000) && (address <= 0xAFFF)) {
		return (prg.rom_8k[1][address & 0x0FFF]);
	}
	if ((address >= 0xB000) && (address <= 0xBFFF)) {
		return (ks7037_prg_B000[address & 0x0FFF]);
	}
	return (openbus);
}
BYTE extcl_save_mapper_KS7037(BYTE mode, BYTE slot, FILE *fp) {
	save_slot_ele(mode, slot, ks7037.ind);
	save_slot_ele(mode, slot, ks7037.reg);

	if (mode == SAVE_SLOT_READ) {
		ks7037_update();
	}

	return (EXIT_OK);
}

static void INLINE ks7037_update(void) {
	WORD value;

	// 0x7000
	value = 0x0F;
	control_bank(info.prg.rom[0].max.banks_4k)
	ks7037_prg_7000 = prg_chip_byte_pnt(0, value << 12);

	// 0x8000 - 0x9000
	value = ks7037.reg[6];
	control_bank(info.prg.rom[0].max.banks_8k)
	map_prg_rom_8k(1, 0, value);
	prg.rom_8k[0] = prg_chip_byte_pnt(prg.rom_chip[0], value << 13);

	// 0xA000
	value = 0xFC;
	control_bank(info.prg.rom[0].max.banks_4k)
	prg.rom_8k[1] = prg_chip_byte_pnt(prg.rom_chip[0], value << 12);

	// 0xB000
	ks7037_prg_B000 = &prg.ram_plus_8k[1 << 12];

	// 0xC000 - 0xD000
	value = ks7037.reg[7];
	control_bank(info.prg.rom[0].max.banks_8k)
	map_prg_rom_8k(1, 2, value);
	prg.rom_8k[2] = prg_chip_byte_pnt(prg.rom_chip[0], value << 13);

	// mirroring
	ntbl.bank_1k[0] = &ntbl.data[(ks7037.reg[2] & 0x01) * 0x400];
	ntbl.bank_1k[1] = &ntbl.data[(ks7037.reg[4] & 0x01) * 0x400];
	ntbl.bank_1k[2] = &ntbl.data[(ks7037.reg[3] & 0x01) * 0x400];
	ntbl.bank_1k[3] = &ntbl.data[(ks7037.reg[5] & 0x01) * 0x400];
}
