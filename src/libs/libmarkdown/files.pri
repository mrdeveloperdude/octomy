MARKDOWN_TOP= $${SRCS}/libs/libmarkdown
UPTIME_TOP= $${SRCS}/libs/libuptime

# From https://github.com/hoedown/hoedown

contains(DEFINES, OC_USE_FEATURE_MARKDOWN){

HEADERS	+= \
	$$MARKDOWN_TOP/markdown/autolink.h \
	$$MARKDOWN_TOP/markdown/buffer.h \
	$$MARKDOWN_TOP/markdown/document.h \
	$$MARKDOWN_TOP/markdown/escape.h \
	$$MARKDOWN_TOP/markdown/html.h \
	$$MARKDOWN_TOP/markdown/stack.h \
	$$MARKDOWN_TOP/markdown/version.h \


SOURCES	+= \
	$$MARKDOWN_TOP/markdown/autolink.c \
	$$MARKDOWN_TOP/markdown/buffer.c \
	$$MARKDOWN_TOP/markdown/document.c \
	$$MARKDOWN_TOP/markdown/escape.c \
	$$MARKDOWN_TOP/markdown/html_blocks.c \
	$$MARKDOWN_TOP/markdown/html.c \
	$$MARKDOWN_TOP/markdown/html_smartypants.c \
	$$MARKDOWN_TOP/markdown/stack.c \
	$$MARKDOWN_TOP/markdown/version.c \


FORMS += \
	$$MARKDOWN_TOP/widgets/ui/MarkdownEditor.ui \
}



#NOTE: These are always included to keep the consumers of this lib happy

HEADERS	+= \
	$$MARKDOWN_TOP/markdown/Markdown.hpp \
	$$MARKDOWN_TOP/markdown/MarkdownHighlighter.hpp \
	$$MARKDOWN_TOP/widgets/markdown/CodeEditor.hpp \
	$$MARKDOWN_TOP/widgets/markdown/LineNumberArea.hpp \
	$$MARKDOWN_TOP/widgets/markdown/MarkdownEditor.hpp \
	$$UPTIME_TOP/uptime/MethodGate.hpp \


SOURCES	+= \
	$$MARKDOWN_TOP/markdown/Markdown.cpp \
	$$MARKDOWN_TOP/markdown/MarkdownHighlighter.cpp \
	$$MARKDOWN_TOP/widgets/markdown/CodeEditor.cpp \
	$$MARKDOWN_TOP/widgets/markdown/LineNumberArea.cpp \
	$$MARKDOWN_TOP/widgets/markdown/MarkdownEditor.cpp \
	$$UPTIME_TOP/uptime/MethodGate.cpp \

