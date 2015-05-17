# -----------------------------------------------------------------------------
# - Wersi specific library                                                    -
# -----------------------------------------------------------------------------
set(SOURCES
	icb.cc
	vcf.cc
	envelope.cc
	wave.cc
	mk1cartridge.cc
	dx10cartridge.cc
)

set(HEADERS
	icb.hh
	vcf.hh
	envelope.hh
	wave.hh
	mk1cartridge.hh
	dx10cartridge.hh
)

add_library(wersi OBJECT ${SOURCES})
