INCLUDES = -I$(top_srcdir)/include

if MINGW32_BUILD
libCEGUITinyXMLParser_la_CPPFLAGS = -DCEGUITINYXMLPARSER_EXPORTS
libCEGUITinyXMLParser_la_LDFLAGS = -no-undefined -avoid-version
libCEGUITinyXMLParser_la_LIBADD = -L$(top_builddir)/src -lCEGUIBase
endif

lib_LTLIBRARIES=libCEGUITinyXMLParser.la

libCEGUITinyXMLParser_la_SOURCES = CEGUITinyXMLParser.cpp \
                                   CEGUITinyXMLParserModule.cpp \
                                   ceguitinyxml/tinystr.cpp \
                                   ceguitinyxml/tinyxml.cpp \
                                   ceguitinyxml/tinyxmlerror.cpp \
                                   ceguitinyxml/tinyxmlparser.cpp

pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/TinyXMLParser
pkginclude_HEADERS = CEGUITinyXMLParser.h CEGUITinyXMLParserModule.h

noinst_HEADERS = ceguitinyxml/tinystr.h \
                 ceguitinyxml/tinyxml.h
