INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/WindowRendererSets/Falagard/include

libCEGUIFalagardWRBase_la_LDFLAGS =

if MINGW32_BUILD
libCEGUIFalagardWRBase_la_CPPFLAGS = -DFALAGARDWRBASE_EXPORTS -DCEGUIWRMODULE_EXPORTS
libCEGUIFalagardWRBase_la_LDFLAGS += -no-undefined -avoid-version
libCEGUIFalagardWRBase_la_LIBADD = -L$(top_builddir)/src/ -lCEGUIBase
endif

libCEGUIFalagardWRBase_la_LDFLAGS += -version-info @CEGUI_VERSION_INFO@

lib_LTLIBRARIES=libCEGUIFalagardWRBase.la

libCEGUIFalagardWRBase_la_SOURCES = FalButton.cpp \
                                  FalToggleButton.cpp \
                                  FalDefault.cpp \
                                  FalEditbox.cpp \
                                  FalFrameWindow.cpp \
                                  FalItemEntry.cpp \
								  FalItemListbox.cpp \
                                  FalListHeader.cpp \
                                  FalListHeaderProperties.cpp \
                                  FalListHeaderSegment.cpp \
                                  FalListbox.cpp \
                                  FalMenubar.cpp \
                                  FalMenuItem.cpp \
                                  FalModule.cpp \
                                  FalMultiColumnList.cpp \
                                  FalMultiLineEditbox.cpp \
                                  FalPopupMenu.cpp \
                                  FalProgressBar.cpp \
                                  FalProgressBarProperties.cpp \
                                  FalScrollablePane.cpp \
                                  FalScrollbar.cpp \
                                  FalScrollbarProperties.cpp \
                                  FalSlider.cpp \
                                  FalSliderProperties.cpp \
                                  FalStatic.cpp \
                                  FalStaticImage.cpp \
                                  FalStaticImageProperties.cpp \
                                  FalStaticProperties.cpp \
                                  FalStaticText.cpp \
                                  FalStaticTextProperties.cpp \
                                  FalSystemButton.cpp \
                                  FalTabButton.cpp \
                                  FalTabControl.cpp \
                                  FalTabControlProperties.cpp \
                                  FalTitlebar.cpp \
                                  FalTree.cpp \
                                  FalTooltip.cpp
