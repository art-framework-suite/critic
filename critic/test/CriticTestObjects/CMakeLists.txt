art_make_library(
  LIBRARY_NAME
    critic_test_CriticTestObjects
  SOURCE
    LiteAssnTestData.cc
    LitePtrTestProduct.cc
  LIBRARIES
    art::art_test_TestObjects
)

art_dictionary(DICTIONARY_LIBRARIES PRIVATE critic::critic_test_CriticTestObjects)
