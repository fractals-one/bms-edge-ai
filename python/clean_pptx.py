"""
Strip all private/confidential data from a PPTX file.

Usage:
    python clean_pptx.py                          # cleans AI_BMS_Demo_Overview.pptx
    python clean_pptx.py  MyPresentation.pptx     # cleans a specific file

Run this AFTER manually editing/saving in PowerPoint to remove any
personal data that Office re-injects (username, company, classification labels).
"""

import os, sys, re, zipfile, shutil, tempfile

def clean_pptx(filepath):
    """Remove all private/confidential metadata from a PPTX file in-place."""
    if not os.path.isfile(filepath):
        print(f"ERROR: File not found: {filepath}")
        return False

    EMPTY_CUSTOM_XML = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
        '<Properties xmlns="http://schemas.openxmlformats.org/officeDocument/2006/custom-properties"'
        ' xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"/>'
    )

    EMPTY_APP_COMPANY = True  # also clear Company from app.xml

    def scrub_core_xml(raw):
        text = raw.decode("utf-8")
        text = re.sub(r"(<dc:creator>)[^<]*(</dc:creator>)", r"\1\2", text)
        text = re.sub(r"(<cp:lastModifiedBy>)[^<]*(</cp:lastModifiedBy>)", r"\1\2", text)
        text = re.sub(r"(<cp:revision>)[^<]*(</cp:revision>)", r"\g<1>1\2", text)
        return text.encode("utf-8")

    def scrub_app_xml(raw):
        text = raw.decode("utf-8")
        text = re.sub(r"(<Company>)[^<]*(</Company>)", r"\1\2", text)
        text = re.sub(r"(<Manager>)[^<]*(</Manager>)", r"\1\2", text)
        return text.encode("utf-8")

    # Keyword audit before cleaning
    keywords = ["enphase", "confidential", "msubramani", "subramani",
                "private", "secret", "proprietary", "restricted"]

    def scrub_slide_xml(raw, name):
        """Remove classification footer text from slide masters/layouts."""
        text = raw.decode("utf-8")
        original = text
        # Remove runs containing classification keywords
        for kw in ["enphase confidential", "enphase"]:
            pattern = r'<a:r>[^<]*<a:rPr[^/]*/>[^<]*<a:t>' + re.escape(kw) + r'</a:t></a:r>'
            text = re.sub(pattern, '', text, flags=re.IGNORECASE)
            # Also handle simpler pattern without rPr attributes
            text = re.sub(r'<a:r><a:rPr[^>]*/><a:t>[^<]*' + re.escape(kw) + r'[^<]*</a:t></a:r>',
                          '', text, flags=re.IGNORECASE)
            # Catch any remaining <a:t> containing the keyword
            text = re.sub(r'<a:t>[^<]*' + re.escape(kw) + r'[^<]*</a:t>',
                          '<a:t></a:t>', text, flags=re.IGNORECASE)
        if text != original:
            cleaned.append(f"{name} (classification text removed)")
        return text.encode("utf-8")

    tmp = tempfile.NamedTemporaryFile(delete=False, suffix=".pptx")
    tmp.close()

    cleaned = []
    with zipfile.ZipFile(filepath, "r") as zin, zipfile.ZipFile(tmp.name, "w") as zout:
        for item in zin.infolist():
            data = zin.read(item.filename)

            if item.filename == "docProps/custom.xml":
                data = EMPTY_CUSTOM_XML.encode("utf-8")
                cleaned.append("custom.xml (classification labels removed)")
            elif item.filename == "docProps/core.xml":
                data = scrub_core_xml(data)
                cleaned.append("core.xml (creator/lastModifiedBy/revision cleared)")
            elif item.filename == "docProps/app.xml":
                data = scrub_app_xml(data)
                cleaned.append("app.xml (Company/Manager cleared)")
            elif item.filename.startswith("ppt/slideMasters/") or \
                 item.filename.startswith("ppt/slideLayouts/") or \
                 item.filename.startswith("ppt/slides/"):
                text_lower = data.decode("utf-8", "ignore").lower()
                if "enphase" in text_lower or "confidential" in text_lower:
                    data = scrub_slide_xml(data, item.filename)

            zout.writestr(item, data)

    shutil.move(tmp.name, filepath)

    # Post-clean verification
    print(f"\nCleaned: {filepath}")
    for c in cleaned:
        print(f"  - {c}")

    print("\nVerification scan...")
    found = False
    with zipfile.ZipFile(filepath, "r") as z:
        for name in z.namelist():
            text = z.read(name).decode("utf-8", "ignore").lower()
            for kw in keywords:
                if kw in text:
                    idx = text.find(kw)
                    ctx = text[max(0, idx-30):idx+len(kw)+30].replace("\n", " ")
                    print(f"  WARNING: '{kw}' still in {name}: ...{ctx}...")
                    found = True

    if not found:
        print("  CLEAN: No private/confidential data found")
    return not found


if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else "AI_BMS_Demo_Overview.pptx"
    if not os.path.isabs(target):
        docs_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "docs")
        target = os.path.join(docs_dir, target)
    clean_pptx(target)
