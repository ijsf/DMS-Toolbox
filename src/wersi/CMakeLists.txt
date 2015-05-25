# -----------------------------------------------------------------------------
# - Wersi specific library                                                    -
# -----------------------------------------------------------------------------
set(SOURCES
	icb.cc
	vcf.cc
	envelope.cc
	wave.cc
	instrumentstore.cc
	mk1cartridge.cc
	dx10cartridge.cc
	dx10device.cc
	sysex.cc
)

set(HEADERS
	icb.hh
	vcf.hh
	envelope.hh
	wave.hh
	instrumentstore.hh
	mk1cartridge.hh
	dx10cartridge.hh
	dx10device.hh
	sysex.hh
)

add_library(wersi OBJECT ${SOURCES})
