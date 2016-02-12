
include(ArtMake)

art_make_library(SOURCE LitePtrTestProduct.cc
  LIBRARIES ${ROOT_CORE} NO_INSTALL
)

art_dictionary(DICTIONARY_LIBRARIES test_CriticTestObjects NO_INSTALL)
