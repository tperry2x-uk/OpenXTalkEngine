/* Copyright (C) 2003-2013 Runtime Revolution Ltd.

This file is part of LiveCode.

LiveCode is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License v3 as published by the Free
Software Foundation.

LiveCode is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with LiveCode.  If not see <http://www.gnu.org/licenses/>.  */

//
// MCField class declarations
//
#ifndef	FIELD_H
#define	FIELD_H

#include "control.h"

#define SCROLL_RATE 100
#define MAX_PASTE_MESSAGES 32

////////////////////////////////////////////////////////////////////////////////

// MW-2012-02-20: [[ FieldExport ]] Structure representing a (flattened) para-
//   graph style used by the export visitor.
struct MCFieldParagraphStyle
{
	bool has_text_align : 1;
	bool has_list_style : 1;
	bool has_first_indent : 1;
	bool has_left_indent : 1;
	bool has_right_indent : 1;
	bool has_space_above : 1;
	bool has_space_below : 1;
	bool has_tabs : 1;
	bool has_background_color : 1;
	bool has_border_width : 1;
	bool has_list_indent : 1;
	bool has_hgrid : 1;
	bool has_vgrid : 1;
	bool has_border_color : 1;
	bool has_dont_wrap : 1;
	bool has_padding : 1;
	bool has_metadata : 1;
	bool has_list_index : 1;
	
	unsigned text_align : 2;
	unsigned list_style : 4;
	unsigned list_depth : 4;
	bool vgrid : 1;
	bool hgrid : 1;
	bool dont_wrap : 1;
	// MW-2012-03-05: [[ HiddenText ]] Whether the paragraph is currently hidden.
	bool hidden : 1;
	
	uint8_t border_width;
	uint8_t padding;
	int16_t list_indent;
	int16_t first_indent;
	int16_t left_indent;
	int16_t right_indent;
	int16_t space_above;
	int16_t space_below;
	uint16_t tab_count;
	uint16_t *tabs;
	uint32_t background_color;
	uint32_t border_color;
	MCNameRef metadata;
	uint16_t list_index;
};

// MW-2012-02-20: [[ FieldExport ]] Structure representing a (flattened) char-
//   acter style used by the export visitor.
struct MCFieldCharacterStyle
{
	bool has_text_color : 1;
	bool has_background_color : 1;
	bool has_link_text : 1;
	bool has_image_source : 1;
	bool has_metadata : 1;
	bool has_text_font : 1;
	bool has_text_style : 1;
	bool has_text_size : 1;
	bool has_text_shift : 1;
	
	uint32_t text_color;
	uint32_t background_color;
	MCStringRef link_text;
	MCStringRef image_source;
	MCStringRef metadata;
	MCNameRef text_font;
	uint2 text_style;
	uint2 text_size;
	int2 text_shift;
};

// MW-2012-02-20: [[ FieldExport ]] Flags describing what kind of export to
//   do.
typedef uint32_t MCFieldExportFlags;
enum
{
	// If set, the callback will be invoked for paragraphs
	kMCFieldExportParagraphs = 1 << 0,
	// If set, the callback will be invoked lines (only possible if the
	// field is open).
	kMCFieldExportLines = 1 << 1,
	// If set, the callback will be invoked for runs
	kMCFieldExportRuns = 1 << 2,
	// If set, the callback will be invoked with paragraph numbering information.
	kMCFieldExportNumbering = 1 << 3,
	// If set, the callback will be invoked with the style information for
	// paragraphs.
	kMCFieldExportParagraphStyles = 1 << 4,
	// If set, the callback will be invoked with the style information for
	// characters.
	kMCFieldExportCharacterStyles = 1 << 5,
	// If set, the callback will be invoked with the flattened style infor-
	// mation taking into account inheritence.
	kMCFieldExportFlattenStyles = 1 << 6,
};

// MW-2012-02-20: [[ FieldExport ]] The event that occured to cause the callback.
enum MCFieldExportEventType
{
	kMCFieldExportEventBeginParagraph,
	kMCFieldExportEventEndParagraph,
	kMCFieldExportEventLineBreak,
	kMCFieldExportEventNativeRun,
	kMCFieldExportEventUnicodeRun
};

// MW-2012-02-20: [[ FieldExport ]] The event data.
struct MCFieldExportEventData
{
	// The current paragraph style (if requested).
	bool has_paragraph_style;
	MCFieldParagraphStyle paragraph_style;
	// The current character style (if requested).
	bool has_character_style;
	MCFieldCharacterStyle character_style;
	// The bytes comprising the current run (if a run event).
	const void *bytes;
	uint32_t byte_count;
	// Whether this event is occuring on the first or last paragraph (or both).
	bool is_first_paragraph, is_last_paragraph;
	// The number of the paragraph (if requested).
	uint32_t paragraph_number;
};

// MW-2012-02-20: [[ FieldExport ]] Callback invoked for each paragraph and run
//   in a field. A paragraph is indicated by 'bytes' being nil; otherwise it is
//   a run. The callback should return 'false' if it wants to terminate.
typedef bool (*MCFieldExportCallback)(void *context, MCFieldExportEventType event_type, const MCFieldExportEventData& event_data);

struct MCInterfaceFlaggedRanges;
struct MCInterfaceFlaggedRange;

////////////////////////////////////////////////////////////////////////////////

class MCField : public MCControl
{
	friend class MCHcfield;
	MCCdata *fdata;
	MCCdata *oldfdata;
	MCCdata *scrolls;
	MCParagraph *paragraphs;
	MCParagraph *oldparagraphs;
	int4 textx;
	int4 texty;
	uint4 textheight;
	uint2 textwidth;
	int2 indent;
	uint2 fixeda;
	uint2 fixedd;
	uint2 fixedheight;
	uint2 foundlength;
	uint2 scrollbarwidth;
	uint2 ntabs;
	uint2 *tabs;
	MCParagraph *curparagraph;
	int4 cury;
	MCParagraph *focusedparagraph;
	int4 focusedy;
	MCParagraph *firstparagraph;
	int4 firsty;
	MCParagraph *lastparagraph;
	int4 foundoffset;
	MCScrollbar *vscrollbar;
	MCScrollbar *hscrollbar;
	MCStringRef label;
	
	static int2 clickx;
	static int2 clicky;
	static int2 goalx;

	static MCRectangle cursorrect;
	static Boolean cursoron;
	static MCField *cursorfield;

	static Boolean extend;
	static Boolean extendwords;
	static Boolean extendlines;
	static Boolean contiguous;
	static int2 narrowmargin;
	static int2 widemargin;
	static Keytranslations emacs_keys[];
	static Keytranslations std_keys[];
	static MCRectangle linkrect;
	static MCBlock *linkstart;
	static MCBlock *linkend;
	static int4 linksi;
	static int4 linkei;
	static int4 composeoffset;
	static uint2 composelength;
	static Boolean composing;
	static uint2 composecursorindex;
	static uint1 composeconvertingsi;
	static uint1 composeconvertingei;

	static MCPropertyInfo kProperties[];
	static MCObjectPropertyTable kPropertyTable;
public:
	MCField();
	MCField(const MCField &fref);
	// virtual functions from MCObject
	virtual ~MCField();
	virtual Chunk_term gettype() const;
	virtual const char *gettypestring();

	virtual const MCObjectPropertyTable *getpropertytable(void) const { return &kPropertyTable; }

	bool visit(MCVisitStyle p_style, uint32_t p_part, MCObjectVisitor* p_visitor);

	virtual void open();
	virtual void close();
	virtual void kfocus();
	virtual void kunfocus();
	virtual Boolean kdown(MCStringRef p_string, KeySym key);
	virtual Boolean mfocus(int2 x, int2 y);
	virtual void munfocus();
	virtual void mdrag(void);
	virtual Boolean mdown(uint2 which);
	virtual Boolean mup(uint2 which);
	virtual Boolean doubledown(uint2 which);
	virtual Boolean doubleup(uint2 which);
	virtual void timer(MCNameRef mptr, MCParameter *params);
	virtual void select();
	virtual uint2 gettransient() const;
	virtual void setrect(const MCRectangle &nrect);
	virtual Exec_stat getprop_legacy(uint4 parid, Properties which, MCExecPoint &, Boolean effective);
	virtual Exec_stat setprop_legacy(uint4 parid, Properties which, MCExecPoint &, Boolean effective);
	virtual void undo(Ustruct *us);
	virtual void recompute();

	// MW-2012-02-14: [[ FontRefs ]] Method called to recompute concrete fonts in the
	//   field.
	virtual bool recomputefonts(MCFontRef parent_font);

	// virtual functions from MCControl
	virtual IO_stat load(IO_handle stream, const char *version);
	virtual IO_stat extendedload(MCObjectInputStream& p_stream, const char *p_version, uint4 p_length);
	virtual IO_stat save(IO_handle stream, uint4 p_part, bool p_force_ext);
	virtual IO_stat extendedsave(MCObjectOutputStream& p_stream, uint4 p_part);

	virtual MCControl *clone(Boolean attach, Object_pos p, bool invisible);

	// MW-2011-09-06: [[ Redraw ]] Added 'sprite' option - if true, ink and opacity are not set.
	virtual void draw(MCDC *dc, const MCRectangle &dirty, bool p_isolated, bool p_sprite);
	
	virtual MCCdata *getdata(uint4 cardid, Boolean clone);
	virtual void replacedata(MCCdata *&data, uint4 newid);
	virtual void compactdata();
	virtual void resetfontindex(MCStack *oldstack);
	virtual Exec_stat hscroll(int4 offset, Boolean doredraw);
	virtual Exec_stat vscroll(int4 offset, Boolean doredraw);
	virtual void readscrollbars();
	virtual void setsbrects();
	virtual void resetscrollbars(Boolean move);
	virtual void getwidgetthemeinfo(MCWidgetInfo &widgetinfo);
	virtual void unlink(MCControl *p_control);

	// MCField functions in fieldf.cc
	static Field_translations trans_lookup(Keytranslations table[], KeySym key, uint2 modifiers);
	static Field_translations lookup_mac_keybinding(KeySym key, uint32_t modifiers);
	
	void redrawcheck(MCRectangle &drect);
	void resetparagraphs();

	MCCdata *getcarddata(MCCdata *&list, uint4 parid, Boolean create);
	MCCdata* getcarddata(uint4 parid) {return getcarddata(fdata, parid, False);}
	
	void openparagraphs();
	void closeparagraphs(MCParagraph *pgptr);
	MCParagraph *getparagraphs(void) {return paragraphs;}
	void gettabs(uint2 *&t, uint2 &n, Boolean &fixed);
	void getlisttabs(int32_t& r_first, int32_t& r_second);

	uint2 getfwidth() const;
	uint2 getfheight() const;
	MCRectangle getfrect() const;

	int32_t getlayoutwidth(void) const;
	int32_t getcontentx(void) const;
	int32_t getcontenty(void) const;
	int32_t gettexty(void) const;
	int32_t getfirstindent(void) const;

	bool getshowlines(void) const;

	void removecursor();
	void drawcursor(MCContext *context, const MCRectangle &drect);
	void positioncursor(Boolean force, Boolean goal, MCRectangle &drect, int4 y);
	void replacecursor(Boolean force, Boolean goal);
	void dragtext();
	void computedrag();
	void drawrect(MCDC *dc, const MCRectangle &dirty);
	void draw3dhilite(MCDC *dc, const MCRectangle &trect);
	void setfocus(int2 x, int2 y);
	uint2 clearhilites();
	void reverse();
	void startselection(int2 x, int2 y, Boolean words);
	void extendselection(int2 x, int2 y);
	void endselection();
	void unselect(Boolean clear, Boolean internal);
	Boolean deleteselection(Boolean force);
	void centerfound();
	void clearfound();
	void updateparagraph(Boolean flow, Boolean redrawall, Boolean dodraw = True);
	void joinparagraphs();
	void fnop(Field_translations function, MCStringRef p_string, KeySym key);
	void finsertnew(Field_translations function, MCStringRef p_string, KeySym key);
	void fdel(Field_translations function, MCStringRef p_string, KeySym key);
	void fhelp(Field_translations function, MCStringRef p_string, KeySym key);
	void fundo(Field_translations function, MCStringRef p_string, KeySym key);
	void fcut(Field_translations function, MCStringRef p_string, KeySym key);
	void fcutline(Field_translations function, MCStringRef p_string, KeySym key);
	void fcopy(Field_translations function, MCStringRef p_string, KeySym key);
	void fpaste(Field_translations function, MCStringRef p_string, KeySym key);
	void ftab(Field_translations function, MCStringRef p_string, KeySym key);
	void ffocus(Field_translations function, MCStringRef p_string, KeySym key);
	void freturn(Field_translations function, MCStringRef p_string, KeySym key);
	void fcenter(Field_translations function, MCStringRef p_string, KeySym key);
	void fmove(Field_translations function, MCStringRef p_string, KeySym key);
	void fscroll(Field_translations function, MCStringRef p_string, KeySym key);
	void setupmenu(MCStringRef p_string, uint2 fheight, Boolean scrolling);
	void setupentry(MCButton *bptr, MCStringRef p_string);
	void typetext(const MCString &newtext);
	void startcomposition();
	void stopcomposition(Boolean del, Boolean force);
	void setcompositioncursoroffset(uint2 coffset);
	void setcompositionconvertingrange(uint1 si,uint1 ei);
	void deletecomposition();
	Boolean getcompositionrect(MCRectangle &r, int2 offset);
	void syncfonttokeyboard();
	void verifyindex(MCParagraph *top, int4 &si, bool p_is_end);
	
	MCParagraph *verifyindices(MCParagraph *top, int4& si, int4& ei);
	
	void indextorect(MCParagraph *top, int4 si, int4 ei, MCRectangle &r);
	void insertparagraph(MCParagraph *newtext);
	// MCField selection functions in fields.cc
	Boolean find(MCExecContext &ctxt, uint4 cardid,
	             Find_mode mode, MCStringRef, Boolean first);
	Exec_stat sort(MCExecContext &ctxt, uint4 parid, Chunk_term type,
	               Sort_type dir, Sort_type form, MCExpression *by);
	// MW-2012-02-08: [[ Field Indices ]] The 'index' parameter, if non-nil, will contain
	//   the 1-based index of the returned paragraph (i.e. the one si resides in).
	MCParagraph *indextoparagraph(MCParagraph *top, int4 &si, int4 &ei, int* index = nil);
	void indextocharacter(int4 &si);
	uint4 ytooffset(int4 y);
	int4 paragraphtoy(MCParagraph *target);
	uint4 getpgsize(MCParagraph *pgptr);

	// MW-2011-02-03: This method returns the 'correct' set of paragraphs for the field given
	//   the specified 'parid'.
	MCParagraph *resolveparagraphs(uint4 parid);

	void setparagraphs(MCParagraph *newpgptr, uint4 parid);
	Exec_stat settext(uint4 parid, MCStringRef p_text, Boolean p_formatted);
	Exec_stat settext_oldstring(uint4 parid, const MCString &newtext, Boolean formatted, Boolean asunicode = False);
	// MW-2012-02-23: [[ PutUnicode ]] Added parameter to specify whether 'is' is unicode or native.
	Exec_stat settextindex(uint4 parid, int4 si, int4 ei, const MCString &s, Boolean undoing, bool asunicode = false);
	Exec_stat settextindex_stringref(uint4 parid, int4 si, int4 ei, MCStringRef s, Boolean undoing);
	void getlinkdata(MCRectangle &r, MCBlock *&sb, MCBlock *&eb);

	// MW-2011-11-23: [[ Array TextStyle ]] Setting/getting text attributes can be indexed by
	//   specific style if which == P_TEXT_STYLE.
	// MW-2012-01-25: [[ ParaStyles ]] Add a line chunk parameter for disambiguating things
	//   like backColor.
	Exec_stat gettextatts(uint4 parid, Properties which, MCExecPoint &, MCNameRef index, Boolean effective, int4 si, int4 ei, bool is_line);
	// MW-2011-12-08: [[ StyledText ]] Change to pass in an ep so that styledText can fetch
	//   the array.
	// MW-2012-01-25: [[ ParaStyles ]] Add a line chunk parameter for disambiguating things
	//   like backColor.
	Exec_stat settextatts(uint4 parid, Properties which, MCExecPoint& ep, MCNameRef index, int4 si, int4 ei, bool is_line);
	Exec_stat seltext(int4 si, int4 ei, Boolean focus, Boolean update = False);
	uint2 hilitedline();
	void hilitedlines(MCExecPoint &ep);
	Exec_stat sethilitedlines(const uint32_t *p_lines, uint32_t p_line_count, Boolean forcescroll = True);
	Exec_stat sethilitedlines(const MCString &,Boolean forcescroll = True);
	void hiliteline(int2 x, int2 y);

	bool locchar(Boolean click, MCStringRef& r_string);
	bool loccharchunk(Boolean click, MCStringRef& r_string);
	bool locchunk(Boolean click, MCStringRef& r_string);
	bool locline(Boolean click, MCStringRef& r_string);
	bool loctext(Boolean click, MCStringRef& r_string);
	Boolean locmark(Boolean wholeline, Boolean wholeword,
	                Boolean click, Boolean chunk, Boolean inc_cr, int4 &si, int4 &ei);

	bool foundchunk(MCStringRef& r_string);
	bool foundline(MCStringRef& r_string);
	bool foundloc(MCStringRef& r_string);
	bool foundtext(MCStringRef& r_string);
	Boolean foundmark(Boolean wholeline, Boolean inc_cr, int4 &si, int4 &ei);

	bool selectedchunk(MCStringRef& r_string);
	bool selectedline(MCStringRef& r_string);
	bool selectedloc(MCStringRef& r_string);
	void selectedtext(MCExecPoint &ep);
	bool selectedtext(MCStringRef& r_string);
	Boolean selectedmark(Boolean wholeline, int4 &si, int4 &ei,
	                     Boolean force, Boolean inc_cr);

	bool returnchunk(int4 si, int4 ei, MCStringRef& r_string);
	bool returnline(int4 si, int4 ei, MCStringRef& r_string);
	bool returnloc(int4 si, MCStringRef& r_string);

	void returntext(MCExecPoint &ep, int4 si, int4 ei);
	bool returntext(int4 si, int4 ei, MCStringRef& r_string);

	void charstoparagraphs(int4 si, int4 ei, MCParagraph*& sp, MCParagraph*& ep, uint4& sl, uint4& el);
	void linestoparagraphs(int4 si, int4 ei, MCParagraph*& sp, MCParagraph*& ep);

	MCParagraph *cloneselection();
	bool pickleselection(MCDataRef& r_string);

	void cuttext();
	void copytext();
	void cuttextindex(uint4 parid, int4 si, int4 ei);
	void copytextindex(uint4 parid, int4 si, int4 ei);
	void pastetext(MCParagraph *newtext, Boolean dodel);
	void movetext(MCParagraph *p_new_text, int4 p_index);
	void deletetext(int4 si, int4 ei);
	MCParagraph *clonetext(int4 si, int4 ei);

	Boolean isautoarm()
	{
		return (flags & F_F_AUTO_ARM) != 0;
	}

	// MCField HTML functions in fieldh.cc
	Exec_stat sethtml(uint4 parid, const MCString &data);
	Exec_stat setrtf(uint4 parid, const MCString &data);
	Exec_stat setstyledtext(uint4 parid, MCExecPoint& ep);
	void setstyledtext(uint32_t part_id, MCArrayRef p_text);
	Exec_stat setpartialtext(uint4 parid, const MCString &data, bool unicode);
	Exec_stat gethtml(uint4 parid, MCExecPoint &ep);
	Exec_stat getparagraphhtml(MCExecPoint &ep, MCParagraph *start, MCParagraph *end);

#ifdef _MACOSX
	Exec_stat getparagraphmacstyles(MCExecPoint &ep, MCParagraph *start, MCParagraph *end, Boolean isunicode);
	Exec_stat getparagraphmacunicodestyles(MCExecPoint &ep, MCParagraph *start, MCParagraph *end);
	MCParagraph *macstyletexttoparagraphs(const MCString &textdata, const MCString &styledata, Boolean isunicode);
	MCParagraph *macunicodestyletexttoparagraphs(const MCString& p_text, const MCString& p_styles);
	static bool macmatchfontname(const char *p_font_name, char p_derived_font_name[]);
#endif

	MCParagraph *rtftoparagraphs(const MCString &data);
	MCParagraph *styledtexttoparagraphs(MCExecPoint& ep);
	MCParagraph *styledtexttoparagraphs(MCArrayRef p_array);
	MCParagraph *texttoparagraphs(const MCString &data, Boolean isunicode);
	
	MCParagraph *parsestyledtextappendparagraph(MCArrayRef p_style, MCNameRef metadata, bool p_split, MCParagraph*& x_paragraphs);
	void parsestyledtextappendblock(MCParagraph *p_paragraph, MCArrayRef p_style, const char *p_initial, const char *p_final, MCStringRef p_metadata, bool p_is_unicode);
	void parsestyledtextblockarray(MCArrayRef p_block_value, MCParagraph*& x_paragraphs);
	void parsestyledtextarray(MCArrayRef p_styled_text, bool p_paragraph_break, MCParagraph*& x_paragraphs);
	
	MCCdata *getcdata(void) {return fdata;}
	
	// MW-2012-01-27: [[ UnicodeChunks ]] Return the contents of the field in a native
	//   compatible way.
	//bool nativizetext(uint4 parid, MCExecPoint& ep, bool p_ascii_only);
	
	// MW-2012-02-08: [[ TextChanged ]] This causes a 'textChanged' message to be sent.
	void textchanged(void);
	
	// MW-2012-02-20: [[ FieldExport ]] Iterates over paragraphs and runs in the field between the
	//   start and end indices, invoking the callback based on the provided flags.
	bool doexport(MCFieldExportFlags flags, uint32_t p_part_id, int32_t start_index, int32_t end_index, MCFieldExportCallback callback, void *context);
	bool doexport(MCFieldExportFlags flags, MCParagraph *p_paragraphs, int32_t start_index, int32_t end_index, MCFieldExportCallback callback, void *context);
	// MW-2012-02-20: [[ FieldExport ]] Convert the content of the field to text, either as unicode
	//   or native encoding.
	void exportastext(uint32_t p_part_id, MCExecPoint& ep, int32_t start_index, int32_t finish_index, bool as_unicode);
	bool exportastext(uint32_t p_part_id, int32_t start_index, int32_t finish_index, MCStringRef& r_string);

	// MW-2012-02-20: [[ FieldExport ]] Convert the content of the field to text, including any list
	//   indices. The output is encoded in either unicode or native.
	void exportasplaintext(uint32_t p_part_id, MCExecPoint& ep, int32_t start_index, int32_t finish_index, bool as_unicode);
	void exportasplaintext(MCExecPoint& ep, MCParagraph *paragraphs, int32_t start_index, int32_t finish_index, bool as_unicode);
	bool exportasplaintext(MCParagraph *p_paragraphs, int32_t p_start_index, int32_t p_finish_index, MCStringRef& r_string);
	bool exportasplaintext(uint32_t p_part_id, int32_t p_start_index, int32_t p_finish_index, MCStringRef& r_string);

	// MW-2012-02-20: [[ FieldExport ]] Convert the content of the field to text, including any list
	//   indices and line breaks.
	void exportasformattedtext(uint32_t p_part_id, MCExecPoint& ep, int32_t start_index, int32_t finish_index, bool as_unicode);
	bool exportasformattedtext(uint32_t p_part_id, int32_t p_start_index, int32_t p_finish_index, MCStringRef& r_string);

	// MW-2012-02-20: [[ FieldExport ]] Convert the content of the field to rtf.
	void exportasrtftext(uint32_t p_part_id, MCExecPoint& ep, int32_t start_index, int32_t finish_index);
	void exportasrtftext(MCExecPoint& ep, MCParagraph *paragraphs, int32_t start_index, int32_t finish_index);
	bool exportasrtftext(uint32_t p_part_id, int32_t p_start_index, int32_t p_finish_index, MCStringRef& r_string);
	bool exportasrtftext(MCParagraph *p_paragraphs, int32_t p_start_index, int32_t p_finish_index, MCStringRef& r_string);

	// MW-2012-02-20: [[ FieldExport ]] Convert the content of the field to (livecode) html.
	void exportashtmltext(uint32_t p_part_id, MCExecPoint& ep, int32_t start_index, int32_t finish_index, bool p_effective);
	void exportashtmltext(MCExecPoint& ep, MCParagraph *paragraphs, int32_t start_index, int32_t finish_index, bool p_effective);
	bool exportashtmltext(uint32_t p_part_id, int32_t p_start_index, int32_t p_finish_index, bool p_effective, MCStringRef& r_string);
	bool exportashtmltext(MCParagraph *p_paragraphs, int32_t p_start_index, int32_t p_finish_index, bool p_effective, MCStringRef& r_string);

	// MW-2012-02-20: [[ FieldExport ]] Convert the content of the field to styled text arrays.
	void exportasstyledtext(uint32_t p_part_id, MCExecPoint& ep, int32_t start_index, int32_t finish_index, bool p_formatted, bool p_effective);
	bool exportasstyledtext(uint32_t p_part_id, int32_t p_start_index, int32_t p_finish_index, bool p_formatted, bool p_effective, MCArrayRef &r_array);

	// MW-2012-03-07: [[ FieldImport ]] Conver the htmlText string to a list of paragraphs.
	MCParagraph *importhtmltext(const MCString& p_data);

	// MW-2012-03-05: [[ FieldImport ]] Add a paragraph with the given styling to the end of the supplied
	//   paragraphs list.
	bool importparagraph(MCParagraph*& x_paragraphs, const MCFieldParagraphStyle *style);
	// MW-2012-03-05: [[ FieldImport ]] Add a block with the given text and styling to the end of the given
	//   paragraph.
	bool importblock(MCParagraph *p_paragraph, const MCFieldCharacterStyle& style, const void *bytes, uint32_t byte_count, bool is_unicode);

	// MW-2012-03-14: [[ RtfParaStyles ]] Modified to take a paragraph style and block style. If block is nil
	//   then it marks a new paragraph (in which case paragraph can be nil to indicate no style).
	static bool converttoparagraphs(void *p_context, const MCTextParagraph *p_paragraph, const MCTextBlock *p_block);

	// MW-2012-02-11: [[ TabWidths ]] The 'which' parameter allows the parsing/formatting
	//   routine to do both stops and widths.
	static bool parsetabstops(Properties which, MCStringRef data, uint16_t*& r_tabs, uint16_t& r_tab_count);
	static void formattabstops(Properties which, MCExecPoint& ep, uint16_t *tabs, uint16_t tab_count);
	
	// MW-2012-02-22: [[ FieldChars ]] Count the number of characters (not bytes) between
	//   start and end in the given field.
	int32_t countchars(uint32_t parid, int32_t si, int32_t ei);
	// MW-2012-02-22: [[ FieldChars ]] Resolve the indices of the char range [start,start + count)
	//   within the given range.
	void resolvechars(uint32_t parid, int32_t& si, int32_t& ei, int32_t start, int32_t count);
	// MW-2012-02-22: [[ FieldChars ]] Convert the field indices to char indices.
	void unresolvechars(uint32_t parid, int32_t& si, int32_t& ei);
	
	// MW-2012-09-04: [[ Bug 9759 ]] This method adjusts the pixmap offsets so that patterns
	//   scroll with the text.
	// MW-2012-11-22: [[ Bug 9759 ]] 'dy' added to allow adjustment for hilite patterns in list
	//   behavior fields.
	void adjustpixmapoffset(MCDC *dc, uint2 index, int4 dy = 0);

	bool imagechanged(MCImage *p_image, bool p_deleting);


	////////// PROPERTY SUPPORT METHODS

	void Redraw(bool reset = false, int4 xoffset = 0, int4 yoffset = 0);
    void UpdateScrollbars(void);
    
    void DoSetInputControl(MCExecContext& ctxt, Properties which, bool setting);
    void DoGetTextState(MCExecContext& ctxt, Properties which, uint32_t part, MCInterfaceTriState& r_state);
    void DoSetTabStops(MCExecContext& ctxt, bool is_relative, uindex_t p_count, uinteger_t *p_tabs);
    
	////////// PROPERTY ACCESSORS

	void GetAutoTab(MCExecContext& ctxt, bool& r_flag);
	void SetAutoTab(MCExecContext& ctxt, bool flag);
	void GetDontSearch(MCExecContext& ctxt, bool& r_flag);
	void SetDontSearch(MCExecContext& ctxt, bool flag);
	void GetDontWrap(MCExecContext& ctxt, bool& r_flag);
	void SetDontWrap(MCExecContext& ctxt, bool flag);
	void GetFixedHeight(MCExecContext& ctxt, bool& r_flag);
	void SetFixedHeight(MCExecContext& ctxt, bool flag);
	void GetLockText(MCExecContext& ctxt, bool& r_flag);
	void SetLockText(MCExecContext& ctxt, bool flag);
    virtual void SetTraversalOn(MCExecContext& ctxt, bool setting);
	void GetSharedText(MCExecContext& ctxt, bool& r_flag);
	void SetSharedText(MCExecContext& ctxt, bool flag);
	void GetShowLines(MCExecContext& ctxt, bool& r_flag);
	void SetShowLines(MCExecContext& ctxt, bool flag);
	void GetHGrid(MCExecContext& ctxt, bool& r_flag);
	void SetHGrid(MCExecContext& ctxt, bool flag);
	void GetVGrid(MCExecContext& ctxt, bool& r_flag);
	void SetVGrid(MCExecContext& ctxt, bool flag);
	void GetStyle(MCExecContext& ctxt, intenum_t& r_style);
	void SetStyle(MCExecContext& ctxt, intenum_t p_style);
	void GetAutoHilite(MCExecContext& ctxt, bool& r_setting);
	void SetAutoHilite(MCExecContext& ctxt, bool setting);
	void GetAutoArm(MCExecContext& ctxt, bool& r_setting);
	void SetAutoArm(MCExecContext& ctxt, bool setting);
	void GetFirstIndent(MCExecContext& ctxt, integer_t& r_indent);
	void SetFirstIndent(MCExecContext& ctxt, integer_t indent);
	void GetWideMargins(MCExecContext& ctxt, bool& r_setting);
	void SetWideMargins(MCExecContext& ctxt, bool setting);
	void GetHScroll(MCExecContext& ctxt, integer_t& r_scroll);
	void SetHScroll(MCExecContext& ctxt, integer_t scroll);
	void GetVScroll(MCExecContext& ctxt, integer_t& r_scroll);
	void SetVScroll(MCExecContext& ctxt, integer_t scroll);
	void GetHScrollbar(MCExecContext& ctxt, bool& r_setting);
	void SetHScrollbar(MCExecContext& ctxt, bool setting);
	void GetVScrollbar(MCExecContext& ctxt, bool& r_setting);
	void SetVScrollbar(MCExecContext& ctxt, bool setting);
	void GetScrollbarWidth(MCExecContext& ctxt, uinteger_t& r_width);
	void SetScrollbarWidth(MCExecContext& ctxt, uinteger_t p_width);
	void GetFormattedWidth(MCExecContext& ctxt, uinteger_t& r_width);
	void GetFormattedHeight(MCExecContext& ctxt, uinteger_t& r_height);
	void GetListBehavior(MCExecContext& ctxt, bool& r_setting);
	void SetListBehavior(MCExecContext& ctxt, bool setting);
	void GetMultipleHilites(MCExecContext& ctxt, bool& r_setting);
	void SetMultipleHilites(MCExecContext& ctxt, bool setting);
	void GetNoncontiguousHilites(MCExecContext& ctxt, bool& r_setting);
	void SetNoncontiguousHilites(MCExecContext& ctxt, bool setting);
	void GetText(MCExecContext& ctxt, uint32_t part, MCStringRef& r_text);
	void SetText(MCExecContext& ctxt, uint32_t part, MCStringRef p_text);
	void GetUnicodeText(MCExecContext& ctxt, uint32_t part, MCDataRef& r_text);
	void SetUnicodeText(MCExecContext& ctxt, uint32_t part, MCDataRef p_text);
	void GetHtmlText(MCExecContext& ctxt, uint32_t part, MCStringRef& r_text);
	void SetHtmlText(MCExecContext& ctxt, uint32_t part, MCStringRef p_text);
	void GetEffectiveHtmlText(MCExecContext& ctxt, uint32_t part, MCStringRef& r_text);
	void GetRtfText(MCExecContext& ctxt, uint32_t part, MCStringRef& r_text);
	void SetRtfText(MCExecContext& ctxt, uint32_t part, MCStringRef p_text);
	void GetStyledText(MCExecContext& ctxt, uint32_t part, MCArrayRef& r_array);
	void SetStyledText(MCExecContext& ctxt, uint32_t part, MCArrayRef p_array);
	void GetEffectiveStyledText(MCExecContext& ctxt, uint32_t part, MCArrayRef& r_array);
	void GetFormattedStyledText(MCExecContext& ctxt, uint32_t part, MCArrayRef& r_array);
	void GetEffectiveFormattedStyledText(MCExecContext& ctxt, uint32_t part, MCArrayRef& r_array);
	void GetPlainText(MCExecContext& ctxt, uint32_t part, MCStringRef& r_string);
	void GetUnicodePlainText(MCExecContext& ctxt, uint32_t part, MCDataRef& r_string);
	void GetFormattedText(MCExecContext& ctxt, uint32_t part, MCStringRef& r_string);
	void SetFormattedText(MCExecContext& ctxt, uint32_t part, MCStringRef p_string);
	void GetUnicodeFormattedText(MCExecContext& ctxt, uint32_t part, MCDataRef& r_string);
	void SetUnicodeFormattedText(MCExecContext& ctxt, uint32_t part, MCDataRef p_string);
	void GetLabel(MCExecContext& ctxt, MCStringRef& r_string);
	void SetLabel(MCExecContext& ctxt, MCStringRef p_string);
	void GetToggleHilite(MCExecContext& ctxt, bool& r_setting);
	void SetToggleHilite(MCExecContext& ctxt, bool setting);
	void GetThreeDHilite(MCExecContext& ctxt, bool& r_setting);
	void SetThreeDHilite(MCExecContext& ctxt, bool setting);
	void GetEncoding(MCExecContext& ctxt, uint32_t part, intenum_t& r_encoding);
    void GetFlagged(MCExecContext& ctxt, uint32_t part, MCInterfaceTriState& r_flagged);
    
    void GetHilitedLines(MCExecContext& ctxt, uindex_t& r_count, uinteger_t*& r_lines);
    void SetHilitedLines(MCExecContext& ctxt, uindex_t p_count, uinteger_t* p_lines);
    void GetFlaggedRanges(MCExecContext& ctxt, uint32_t p_part, MCInterfaceFlaggedRanges& r_ranges);
    void SetFlaggedRanges(MCExecContext& ctxt, uint32_t p_part, const MCInterfaceFlaggedRanges& p_ranges); 
    void SetTabStops(MCExecContext& ctxt, uindex_t p_count, uinteger_t *p_tabs);
    void GetTabStops(MCExecContext& ctxt, uindex_t& r_count, uinteger_t*& r_tabs);
    void SetTabWidths(MCExecContext& ctxt, uindex_t p_count, uinteger_t *p_tabs);
    void GetTabWidths(MCExecContext& ctxt, uindex_t& r_count, uinteger_t*& r_tabs);
    void GetPageHeights(MCExecContext& ctxt, uindex_t& r_count, uinteger_t*& r_heights);
    
    virtual void SetShadow(MCExecContext& ctxt, const MCInterfaceShadow& p_shadow);
    virtual void SetShowBorder(MCExecContext& ctxt, bool setting);
    virtual void SetTextHeight(MCExecContext& ctxt, uinteger_t* height);
    virtual void SetTextFont(MCExecContext& ctxt, MCStringRef font);
    virtual void SetTextSize(MCExecContext& ctxt, uinteger_t* size);
    virtual void SetTextStyle(MCExecContext& ctxt, const MCInterfaceTextStyle& p_style);
    virtual void SetBorderWidth(MCExecContext& ctxt, uinteger_t width);
    virtual void Set3D(MCExecContext& ctxt, bool setting);
    virtual void SetOpaque(MCExecContext& ctxt, bool setting);
    virtual void SetEnabled(MCExecContext& ctxt, uint32_t part, bool setting);
	virtual void SetDisabled(MCExecContext& ctxt, uint32_t part, bool setting);
    
	virtual void SetLeftMargin(MCExecContext& ctxt, integer_t p_margin);
	virtual void SetRightMargin(MCExecContext& ctxt, integer_t p_margin);
	virtual void SetTopMargin(MCExecContext& ctxt, integer_t p_margin);
	virtual void SetBottomMargin(MCExecContext& ctxt, integer_t p_margin);
    virtual void SetMargins(MCExecContext& ctxt, const MCInterfaceMargins& p_margins);
	virtual void SetWidth(MCExecContext& ctxt, uinteger_t value);
	virtual void SetHeight(MCExecContext& ctxt, uinteger_t value);
    virtual void SetEffectiveWidth(MCExecContext& ctxt, uinteger_t value);
	virtual void SetEffectiveHeight(MCExecContext& ctxt, uinteger_t value);
    virtual void SetRectangle(MCExecContext& ctxt, MCRectangle p_rect);
    virtual void SetEffectiveRectangle(MCExecContext& ctxt, MCRectangle p_rect);
};
#endif
