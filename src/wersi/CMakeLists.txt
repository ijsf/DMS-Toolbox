# -----------------------------------------------------------------------------
# - Wersi specific library                                                    -
# -----------------------------------------------------------------------------
set(SOURCES
	icb.cc
	vcf.cc
	envelope.cc
	wave.cc
)

set(HEADERS
	icb.hh
	vcf.hh
	envelope.hh
	wave.hh
)

add_library(wersi OBJECT ${SOURCES})
