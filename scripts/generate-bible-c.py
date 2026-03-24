#!/usr/bin/env python3
"""
generate-bible-c.py  —  Generate src/app/bible/bible.c from lib/esv-bible/json/ESV.json.

The ESV.json structure is:
  { "books": { "Genesis": [chapter, ...], ... } }
  where chapter = [verse, ...] and verse = [[word_or_tokens, ...], ...]
  Text is reconstructed by joining chunk[0] for each non-list chunk in the verse.

Usage:
    python3 scripts/generate-bible-c.py [input.json] [output.c]

  input.json  defaults to lib/esv-bible/json/ESV.json
  output.c    defaults to src/app/bible/bible.c
"""

import json
import re
import sys
import os
from collections import OrderedDict

BOOK_ORDER = [
    "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
    "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel",
    "1 Kings", "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra",
    "Nehemiah", "Esther", "Job", "Psalms", "Proverbs",
    "Ecclesiastes", "Song of Solomon", "Isaiah", "Jeremiah", "Lamentations",
    "Ezekiel", "Daniel", "Hosea", "Joel", "Amos",
    "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk",
    "Zephaniah", "Haggai", "Zechariah", "Malachi",
    "Matthew", "Mark", "Luke", "John", "Acts",
    "Romans", "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians",
    "Philippians", "Colossians", "1 Thessalonians", "2 Thessalonians",
    "1 Timothy", "2 Timothy", "Titus", "Philemon",
    "Hebrews", "James", "1 Peter", "2 Peter",
    "1 John", "2 John", "3 John", "Jude", "Revelation",
]


# Map ESV JSON book names → canonical display names
ESV_BOOK_MAP = {
    "I Samuel":           "1 Samuel",
    "II Samuel":          "2 Samuel",
    "I Kings":            "1 Kings",
    "II Kings":           "2 Kings",
    "I Chronicles":       "1 Chronicles",
    "II Chronicles":      "2 Chronicles",
    "I Corinthians":      "1 Corinthians",
    "II Corinthians":     "2 Corinthians",
    "I Thessalonians":    "1 Thessalonians",
    "II Thessalonians":   "2 Thessalonians",
    "I Timothy":          "1 Timothy",
    "II Timothy":         "2 Timothy",
    "I Peter":            "1 Peter",
    "II Peter":           "2 Peter",
    "I John":             "1 John",
    "II John":            "2 John",
    "III John":           "3 John",
    "Revelation of John": "Revelation",
}


def verse_text(raw_verse):
    """Reconstruct plain text from ESV token list."""
    words = []
    for chunk in raw_verse:
        if not isinstance(chunk[0], list):
            words.append(chunk[0])
    text = " ".join(words)
    # remove space before punctuation
    text = re.sub(r'\s([?.,:;!\')])', r'\1', text)
    # remove space before closing quote when preceded by punctuation
    text = re.sub(r'\s(")', r'\1', text)
    return text.strip()


def load_esv(path):
    with open(path, encoding="utf-8") as f:
        data = json.load(f)

    books_raw = data["books"]
    # remap ESV names to canonical names
    books_remapped = {ESV_BOOK_MAP.get(k, k): v for k, v in books_raw.items()}
    books = OrderedDict()

    for book_name in BOOK_ORDER:
        if book_name not in books_remapped:
            print(f"  WARNING: '{book_name}' not found in JSON", file=sys.stderr)
            continue
        chapters = {}
        for ch_idx, chapter in enumerate(books_remapped[book_name]):
            ch_no = ch_idx + 1
            verses = {}
            for vs_idx, verse in enumerate(chapter):
                vs_no = vs_idx + 1
                verses[vs_no] = verse_text(verse)
            chapters[ch_no] = verses
        books[book_name] = chapters

    return books


def build_tables(books):
    book_list    = []
    verse_list   = []
    string_pool  = bytearray()
    all_chapters = []

    for book_idx, (book_name, chapters) in enumerate(books.items()):
        verse_start   = len(verse_list)
        chapter_start = len(all_chapters)

        for ch_no in sorted(chapters):
            ch_verse_start = len(verse_list) - verse_start
            for vs_no in sorted(chapters[ch_no]):
                text = chapters[ch_no][vs_no]
                text_offset = len(string_pool)
                string_pool.extend(text.encode("utf-8") + b"\x00")
                verse_list.append({
                    "book_idx":    book_idx,
                    "chapter":     ch_no,
                    "verse":       vs_no,
                    "text_offset": text_offset,
                })
            all_chapters.append({
                "verse_start": verse_start + ch_verse_start,
                "verse_count": len(chapters[ch_no]),
            })

        book_list.append({
            "name":          book_name,
            "verse_start":   verse_start,
            "verse_count":   len(verse_list) - verse_start,
            "chapter_count": len(chapters),
            "chapter_start": chapter_start,
        })

    return book_list, verse_list, string_pool, all_chapters


def escape_c(s):
    return s.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n").replace("\r", "")


def emit(book_list, verse_list, string_pool, all_chapters, out_path):
    lines = []
    a = lines.append

    a('/* bible.c  —  AUTO-GENERATED, DO NOT EDIT.')
    a(' * Regenerate: python3 scripts/generate-bible-c.py [input.json] [output.c]')
    a(' */')
    a('#include "bible.h"')
    a('#include <stdint.h>')
    a('')

    nbooks  = len(book_list)
    nverses = len(verse_list)
    npool   = len(string_pool)

    # count variables
    a(f'const uint8_t  bible_book_count_val  = {nbooks}u;')
    a(f'const uint16_t bible_verse_count_val = {nverses}u;')
    a('')

    # string pool
    a(f'/* {npool}-byte string pool */')
    a(f'const char bible_text[{npool}] = {{')
    COLS = 16
    for i in range(0, npool, COLS):
        chunk = string_pool[i:i + COLS]
        a('  ' + ', '.join(f'0x{b:02x}' for b in chunk) + ',')
    a('};')
    a('')

    # flat verse table
    a(f'/* {nverses} verses */')
    a(f'const bible_verse_t bible_verses[{nverses}] = {{')
    for v in verse_list:
        a(f'  {{ {v["book_idx"]:3}, {v["chapter"]:3}, {v["verse"]:3}, {v["text_offset"]:7} }},')
    a('};')
    a('')

    # chapter table (file-local)
    a(f'/* {len(all_chapters)} chapters */')
    a(f'static const bible_chapter_t s_chapters[{len(all_chapters)}] = {{')
    for c in all_chapters:
        a(f'  {{ {c["verse_start"]:6}, {c["verse_count"]:3} }},')
    a('};')
    a('')

    # book table
    a(f'/* {nbooks} books */')
    a(f'const bible_book_t bible_books[{nbooks}] = {{')
    for b in book_list:
        a(f'  {{ "{escape_c(b["name"])}", {b["verse_start"]:6}, {b["verse_count"]:5},'
          f' {b["chapter_count"]:3}, {b["chapter_start"]:5} }},')
    a('};')
    a('')

    # bible_chapter_verse_count
    a('uint8_t bible_chapter_verse_count(uint8_t book_idx, uint8_t chapter)')
    a('{')
    a('  if(book_idx >= bible_book_count_val || chapter == 0) return 0;')
    a('  const bible_book_t* b = &bible_books[book_idx];')
    a('  if(chapter > b->chapter_count) return 0;')
    a('  return s_chapters[b->chapter_start + chapter - 1].verse_count;')
    a('}')
    a('')

    # bible_find_verse
    a('uint16_t bible_find_verse(uint8_t book_idx, uint8_t chapter, uint8_t verse)')
    a('{')
    a('  if(book_idx >= bible_book_count_val || chapter == 0) return UINT16_MAX;')
    a('  const bible_book_t* b = &bible_books[book_idx];')
    a('  if(chapter > b->chapter_count) return UINT16_MAX;')
    a('  const bible_chapter_t* c = &s_chapters[b->chapter_start + chapter - 1];')
    a('  for(uint16_t i = c->verse_start; i < c->verse_start + c->verse_count; i++) {')
    a('    if(bible_verses[i].verse == verse) return i;')
    a('  }')
    a('  return UINT16_MAX;')
    a('}')
    a('')

    os.makedirs(os.path.dirname(os.path.abspath(out_path)), exist_ok=True)
    with open(out_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))
    print(f"  wrote {out_path}  ({len(verse_list)} verses, {len(string_pool)} bytes pool)")


def main():
    input_json = sys.argv[1] if len(sys.argv) > 1 else "lib/esv-bible/json/ESV.json"
    out_path   = sys.argv[2] if len(sys.argv) > 2 else "src/app/bible/bible.c"

    print(f"Loading {input_json}...")
    books = load_esv(input_json)
    print(f"  {len(books)} books")

    book_list, verse_list, string_pool, all_chapters = build_tables(books)
    print(f"  {len(verse_list)} verses, {len(string_pool)}-byte string pool")

    emit(book_list, verse_list, string_pool, all_chapters, out_path)
    print("Done.")


if __name__ == "__main__":
    main()
