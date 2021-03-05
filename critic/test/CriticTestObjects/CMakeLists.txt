cet_make_library(SOURCE LiteAssnTestData.cc LitePtrTestProduct.cc
  LIBRARIES PUBLIC art_test::TestObjects canvas::canvas
)

art_dictionary(DICTIONARY_LIBRARIES
  critic_test::CriticTestObjects
  cetlib::cetlib
)
