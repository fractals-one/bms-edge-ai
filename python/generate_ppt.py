"""
Generate an executive-grade PowerPoint presentation explaining the
STSW-L9961BMS AI Demo code flow and algorithm, with simulated Tera Term
terminal screenshots rendered via Pillow.
"""

import os, io
from PIL import Image, ImageDraw, ImageFont
from pptx import Presentation
from pptx.util import Inches, Pt, Emu
from pptx.dml.color import RGBColor
from pptx.enum.text import PP_ALIGN, MSO_ANCHOR
from pptx.enum.shapes import MSO_SHAPE

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DOCS_DIR = os.path.join(os.path.dirname(BASE_DIR), "docs")

# ─────────────────────────── colour palette ───────────────────────────
BG_DARK    = RGBColor(0x0F, 0x17, 0x2A)
BG_CARD    = RGBColor(0x17, 0x20, 0x3A)
ACCENT     = RGBColor(0x03, 0xA9, 0xF4)
GREEN      = RGBColor(0x4C, 0xAF, 0x50)
CYAN       = RGBColor(0x00, 0xE5, 0xFF)
ORANGE     = RGBColor(0xFF, 0x98, 0x00)
RED        = RGBColor(0xF4, 0x43, 0x36)
YELLOW     = RGBColor(0xFF, 0xD5, 0x4F)
WHITE      = RGBColor(0xFF, 0xFF, 0xFF)
LGRAY      = RGBColor(0x90, 0xA4, 0xAE)
DGRAY      = RGBColor(0x37, 0x47, 0x4F)
ST_BLUE    = RGBColor(0x03, 0x23, 0x4B)

prs = Presentation()
prs.slide_width  = Inches(13.333)
prs.slide_height = Inches(7.5)

# ─────────────────────────── helpers ──────────────────────────────────
def bg(slide, color=BG_DARK):
    slide.background.fill.solid()
    slide.background.fill.fore_color.rgb = color

def txb(slide, l, t, w, h, txt, sz=14, c=WHITE, b=False, al=PP_ALIGN.LEFT, fn="Calibri"):
    box = slide.shapes.add_textbox(Inches(l), Inches(t), Inches(w), Inches(h))
    tf = box.text_frame; tf.word_wrap = True
    p = tf.paragraphs[0]; p.text = txt
    p.font.size = Pt(sz); p.font.color.rgb = c; p.font.bold = b; p.font.name = fn
    p.alignment = al
    return box

def mtxb(slide, l, t, w, h, lines, sz=13, c=WHITE, fn="Calibri"):
    box = slide.shapes.add_textbox(Inches(l), Inches(t), Inches(w), Inches(h))
    tf = box.text_frame; tf.word_wrap = True
    for i,(txt, tc, tb, ts) in enumerate(lines):
        p = tf.paragraphs[0] if i == 0 else tf.add_paragraph()
        p.text = txt
        p.font.size = Pt(ts or sz); p.font.color.rgb = tc or c
        p.font.bold = tb; p.font.name = fn; p.space_after = Pt(2)
    return box

def rrect(slide, l, t, w, h, fc, txt="", sz=12, tc=WHITE, b=False):
    s = slide.shapes.add_shape(MSO_SHAPE.ROUNDED_RECTANGLE,
                               Inches(l), Inches(t), Inches(w), Inches(h))
    s.fill.solid(); s.fill.fore_color.rgb = fc; s.line.fill.background()
    if txt:
        tf = s.text_frame; tf.word_wrap = True
        tf.paragraphs[0].alignment = PP_ALIGN.CENTER
        p = tf.paragraphs[0]; p.text = txt
        p.font.size = Pt(sz); p.font.color.rgb = tc; p.font.bold = b; p.font.name = "Calibri"
    return s

def arrow(slide, x1, y1, x2, y2, c=ACCENT, w=Pt(2.5)):
    cn = slide.shapes.add_connector(1, Inches(x1), Inches(y1), Inches(x2), Inches(y2))
    cn.line.color.rgb = c; cn.line.width = w

def chevron(slide, l, t, w, h, fc, txt="", sz=11, tc=WHITE, b=False):
    s = slide.shapes.add_shape(MSO_SHAPE.CHEVRON,
                               Inches(l), Inches(t), Inches(w), Inches(h))
    s.fill.solid(); s.fill.fore_color.rgb = fc; s.line.fill.background()
    if txt:
        tf = s.text_frame; tf.word_wrap = True
        tf.paragraphs[0].alignment = PP_ALIGN.CENTER
        p = tf.paragraphs[0]; p.text = txt
        p.font.size = Pt(sz); p.font.color.rgb = tc; p.font.bold = b; p.font.name = "Calibri"
    return s

def slide_number_footer(slide, num, total, extra=""):
    footer = f"{'  ' + extra + '  |  ' if extra else ''}Slide {num}/{total}"
    txb(slide, 10.5, 7.1, 2.5, 0.3, footer, sz=9, c=LGRAY, al=PP_ALIGN.RIGHT)

# ─── terminal image generator ────────────────────────────────────────
TERM_BG   = (0, 0, 0)
TERM_FONT = None   # will fallback to default

def _color(name):
    m = {"white":(255,255,255), "green":(0,255,0), "cyan":(0,255,255),
         "yellow":(255,255,0), "blue_bg":(0,70,180), "green_bg":(0,160,0),
         "red_bg":(200,0,0), "gray":(160,160,160), "magenta":(200,0,200)}
    return m.get(name, (255,255,255))

def render_terminal(lines, width=720, title="COM3 - Tera Term VT"):
    """Render a list of (text, fg_color_name, bg_color_name_or_None) into a PNG image."""
    line_h = 18
    pad = 12
    title_h = 28
    img_h = title_h + pad * 2 + len(lines) * line_h + 4
    img = Image.new("RGB", (width, img_h), TERM_BG)
    draw = ImageDraw.Draw(img)

    # title bar
    draw.rectangle([0, 0, width, title_h], fill=(0, 70, 180))
    try:
        tfont = ImageFont.truetype("consola.ttf", 13)
    except:
        tfont = ImageFont.load_default()
    draw.text((8, 6), title, fill=(255,255,255), font=tfont)

    try:
        font = ImageFont.truetype("consola.ttf", 13)
    except:
        font = ImageFont.load_default()

    y = title_h + pad
    for parts in lines:
        x = pad
        for segment in parts:
            if len(segment) == 2:
                txt, fg = segment
                bg_c = None
            else:
                txt, fg, bg_c = segment
            fc = _color(fg)
            if bg_c:
                bc = _color(bg_c)
                bbox = font.getbbox(txt)
                tw = bbox[2] - bbox[0]
                draw.rectangle([x, y, x + tw + 2, y + line_h], fill=bc)
            draw.text((x, y), txt, fill=fc, font=font)
            bbox = font.getbbox(txt)
            x += bbox[2] - bbox[0]
        y += line_h

    buf = io.BytesIO()
    img.save(buf, format="PNG")
    buf.seek(0)
    return buf

def add_terminal_image(slide, left, top, width, lines, title="COM3 - Tera Term VT"):
    buf = render_terminal(lines, width=int(width * 96), title=title)
    slide.shapes.add_picture(buf, Inches(left), Inches(top), Inches(width))


# =====================================================================
#  SLIDE 1 — TITLE SLIDE
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
TOTAL_SLIDES = 9

# accent bar at top
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

# Main title
txb(s, 1.0, 1.8, 11, 1.0,
    "AI-Powered Battery Management System",
    sz=40, c=WHITE, b=True, al=PP_ALIGN.CENTER)

txb(s, 1.0, 2.8, 11, 0.6,
    "Real-Time State of Charge & State of Health Estimation on STM32",
    sz=20, c=LGRAY, al=PP_ALIGN.CENTER)

# Divider
rrect(s, 5.5, 3.6, 2.3, 0.04, ACCENT)

txb(s, 1.0, 3.9, 11, 0.5,
    "STSW-L9961BMS  |  ST Edge AI  |  Cortex-M0+",
    sz=16, c=CYAN, al=PP_ALIGN.CENTER, b=True)

# Three value-prop boxes
props = [
    ("On-Device AI", "Neural network runs\ndirectly on STM32 MCU", GREEN),
    ("Real-Time", "Inference every 300ms\ndata acquisition cycle", ACCENT),
    ("Production Ready", "Filtered output with\nsaturation & averaging", ORANGE),
]
for i, (title, desc, color) in enumerate(props):
    x = 2.0 + i * 3.5
    rrect(s, x, 4.7, 2.8, 1.8, BG_CARD)
    rrect(s, x, 4.7, 2.8, 0.06, color)
    txb(s, x + 0.2, 4.9, 2.4, 0.4, title, sz=18, c=color, b=True, al=PP_ALIGN.CENTER)
    txb(s, x + 0.2, 5.4, 2.4, 0.8, desc, sz=13, c=LGRAY, al=PP_ALIGN.CENTER)

txb(s, 1.0, 6.8, 11, 0.3,
    "STMicroelectronics",
    sz=11, c=LGRAY, al=PP_ALIGN.CENTER)
slide_number_footer(s, 1, TOTAL_SLIDES)


# =====================================================================
#  SLIDE 2 — HARDWARE PLATFORM
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 10, 0.6, "Hardware Platform  —  STEVAL-L99615C Evaluation Kit", sz=30, c=WHITE, b=True)
txb(s, 0.6, 0.85, 10, 0.35,
    "Industrial BMS evaluation board with L9961 IC and STM32 Nucleo-64 development board",
    sz=14, c=LGRAY)

# ─── Left: Actual board photo + block diagram ───
# Try to load actual board photo from local file or download from ST
def get_board_image():
    """Load actual board photo from local file (any common extension)."""
    for ext in ("png", "jpg", "jpeg", "bmp", "webp"):
        p = os.path.join(DOCS_DIR, f"board_photo.{ext}")
        if os.path.exists(p):
            return p
    return None

def render_board_diagram():
    """Render a detailed block diagram of the STEVAL-L99615C + NUCLEO-G071RB stack."""
    W, H = 520, 500
    img = Image.new("RGB", (W, H), (15, 23, 42))
    draw = ImageDraw.Draw(img)
    try:
        font = ImageFont.truetype("consola.ttf", 12)
        font_sm = ImageFont.truetype("consola.ttf", 10)
        font_lg = ImageFont.truetype("consola.ttf", 14)
        font_title = ImageFont.truetype("consola.ttf", 16)
    except:
        font = font_sm = font_lg = font_title = ImageFont.load_default()

    # L9961 expansion board (top)
    draw.rounded_rectangle([30, 20, 490, 245], radius=12, fill=(0, 50, 120), outline=(0, 150, 255), width=2)
    draw.text((130, 30), "STEVAL-L99615C", fill=(0, 200, 255), font=font_title)
    draw.text((110, 55), "L9961 BMS Expansion Board", fill=(200, 200, 200), font=font)
    # L9961 IC
    draw.rectangle([190, 85, 330, 165], fill=(20, 30, 60), outline=(0, 150, 255), width=2)
    draw.text((220, 100), "L9961", fill=(0, 220, 255), font=font_lg)
    draw.text((210, 125), "5-Cell BMS IC", fill=(180, 180, 180), font=font_sm)
    draw.text((208, 142), "VFQFPN-32", fill=(140, 140, 140), font=font_sm)
    # Battery connector
    draw.rounded_rectangle([50, 90, 160, 160], radius=4, fill=(80, 40, 0), outline=(255, 150, 0), width=2)
    draw.text((55, 100), "5-Cell", fill=(255, 200, 0), font=font)
    draw.text((55, 118), "Battery", fill=(255, 200, 0), font=font)
    draw.text((55, 136), "Connector", fill=(255, 150, 0), font=font_sm)
    # MOSFETs
    draw.rounded_rectangle([360, 90, 470, 130], radius=4, fill=(50, 20, 20), outline=(200, 50, 50), width=1)
    draw.text((370, 97), "CHG/DCHG", fill=(255, 100, 100), font=font_sm)
    draw.text((370, 112), "MOSFETs", fill=(200, 100, 100), font=font_sm)
    # NTC
    draw.rounded_rectangle([360, 140, 470, 170], radius=4, fill=(50, 50, 20), outline=(200, 200, 50), width=1)
    draw.text((375, 147), "NTC Temp", fill=(255, 255, 100), font=font_sm)
    # Fuse
    draw.rounded_rectangle([360, 180, 470, 210], radius=4, fill=(50, 20, 50), outline=(200, 50, 200), width=1)
    draw.text((370, 188), "Fuse Emu", fill=(255, 100, 255), font=font_sm)
    # I2C label between boards
    draw.rectangle([200, 240, 320, 268], fill=(30, 30, 60))
    draw.text((215, 245), "I2C + SPI", fill=(0, 230, 255), font=font)
    # Stacking arrows
    draw.line([120, 245, 120, 260], fill=(0, 200, 200), width=2)
    draw.line([400, 245, 400, 260], fill=(0, 200, 200), width=2)

    # Nucleo board (bottom)
    draw.rounded_rectangle([30, 260, 490, 480], radius=12, fill=(0, 80, 50), outline=(0, 200, 80), width=2)
    draw.text((140, 270), "NUCLEO-G071RB", fill=(0, 255, 80), font=font_title)
    draw.text((130, 295), "STM32G071RB  (Cortex-M0+)", fill=(200, 200, 200), font=font)
    # MCU chip
    draw.rectangle([190, 320, 330, 400], fill=(30, 40, 30), outline=(0, 180, 80), width=2)
    draw.text((210, 340), "STM32G071", fill=(0, 255, 100), font=font_lg)
    draw.text((225, 365), "Cortex-M0+", fill=(180, 180, 180), font=font_sm)
    draw.text((225, 380), "128KB Flash", fill=(180, 180, 180), font=font_sm)
    # USB
    draw.rounded_rectangle([60, 420, 140, 460], radius=4, fill=(60, 60, 60), outline=(150, 150, 150))
    draw.text((75, 430), "USB", fill=(255, 255, 255), font=font_sm)
    # LEDs
    draw.ellipse([370, 420, 385, 435], fill=(0, 255, 0))
    draw.text((390, 422), "LD2", fill=(150, 150, 150), font=font_sm)
    draw.ellipse([370, 445, 385, 460], fill=(255, 0, 0))
    draw.text((390, 447), "PWR", fill=(150, 150, 150), font=font_sm)
    # Morpho connectors
    draw.rectangle([35, 310, 55, 460], fill=(40, 40, 40), outline=(100, 100, 100))
    draw.text((33, 470), "CN7", fill=(120, 120, 120), font=font_sm)
    draw.rectangle([465, 310, 485, 460], fill=(40, 40, 40), outline=(100, 100, 100))
    draw.text((463, 470), "CN10", fill=(120, 120, 120), font=font_sm)

    buf = io.BytesIO()
    img.save(buf, format="PNG")
    buf.seek(0)
    return buf

# Add actual photo if available
photo_path = get_board_image()
if photo_path:
    s.shapes.add_picture(photo_path, Inches(0.6), Inches(1.4), Inches(4.8))
else:
    # Fallback: use block diagram as main image
    fallback_buf = render_board_diagram()
    s.shapes.add_picture(fallback_buf, Inches(0.6), Inches(1.4), Inches(4.8))

# Add block diagram below the photo
diagram_buf = render_board_diagram()
s.shapes.add_picture(diagram_buf, Inches(0.6), Inches(4.7), Inches(4.8))
txb(s, 0.6, 4.45, 4.8, 0.25, "STEVAL-L99615C + NUCLEO-G071RB  (Block Diagram)",
    sz=10, c=LGRAY, al=PP_ALIGN.CENTER)

# ─── Right: Hardware Details ───
rrect(s, 6.0, 1.4, 6.8, 5.7, BG_CARD)
rrect(s, 6.0, 1.4, 6.8, 0.06, ACCENT)
txb(s, 6.2, 1.55, 6.4, 0.35, "STEVAL-L99615C  —  Evaluation Kit", sz=18, c=ACCENT, b=True)

# Kit description
txb(s, 6.2, 2.0, 6.4, 0.6,
    "The STEVAL-L99615C is an evaluation kit composed of an L9961 BMS expansion board\n"
    "stacked on a NUCLEO-G071RB STM32 Nucleo-64 development board via morpho connectors.",
    sz=12, c=LGRAY)

# L9961 IC card
rrect(s, 6.2, 2.7, 6.4, 1.6, RGBColor(0x1A,0x23,0x7E))
mtxb(s, 6.3, 2.75, 6.2, 1.5, [
    ("L9961 BMS IC  —  Key Features", YELLOW, True, 14),
    ("", None, False, 4),
    ("Monitors up to 5 Li-ion cells in series configuration", WHITE, False, 12),
    ("High-precision ADC for cell voltage, stack voltage, NTC temperature", WHITE, False, 12),
    ("High-accuracy Current Sense Amplifier (CSA) with Coulomb counter", WHITE, False, 12),
    ("Passive cell balancing (all cells simultaneously)", WHITE, False, 12),
    ("Dual pre-driver for charge/discharge MOSFETs", WHITE, False, 12),
    ("OV/UV, OT/UT, overcurrent & short-circuit protection", ORANGE, False, 12),
    ("I2C interface to host MCU  |  NVM for safety configs", WHITE, False, 12),
])

# MCU card
rrect(s, 6.2, 4.5, 3.0, 1.3, RGBColor(0x1B,0x5E,0x20))
mtxb(s, 6.3, 4.55, 2.8, 1.2, [
    ("STM32G071RB MCU", YELLOW, True, 13),
    ("", None, False, 4),
    ("Arm Cortex-M0+ @ 64 MHz", WHITE, False, 11),
    ("128 KB Flash / 36 KB RAM", WHITE, False, 11),
    ("NUCLEO-G071RB board", CYAN, False, 11),
    ("ST-LINK/V2-1 debugger", WHITE, False, 11),
])

# Board features card
rrect(s, 9.4, 4.5, 3.4, 1.3, RGBColor(0x00,0x60,0x64))
mtxb(s, 9.5, 4.55, 3.2, 1.2, [
    ("Expansion Board Features", YELLOW, True, 13),
    ("", None, False, 4),
    ("5-cell battery pack connector", WHITE, False, 11),
    ("Embedded battery simulator", CYAN, False, 11),
    ("Onboard fuse emulator", WHITE, False, 11),
    ("Charge / Discharge MOSFETs", WHITE, False, 11),
])

# Software card
rrect(s, 6.2, 6.0, 6.4, 1.0, RGBColor(0x4A,0x14,0x8C))
mtxb(s, 6.3, 6.05, 6.2, 0.9, [
    ("STSW-L9961BMS  —  Software Package", YELLOW, True, 13),
    ("4 application demos: Cell Monitoring, Coulomb Counting, Passive Balancing, Kalman SoC/SoH", WHITE, False, 11),
    ("+ NEW AI Demo (this project):  Neural network based SoC/SoH estimation", GREEN, True, 12),
    ("STM32Cube HAL + L9961 firmware driver  |  UART output to Tera Term", LGRAY, False, 11),
])

slide_number_footer(s, 2, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")


# =====================================================================
#  SLIDE 3 — MODEL TRAINING PIPELINE
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 8, 0.6, "AI Model Training Pipeline", sz=30, c=WHITE, b=True)
txb(s, 0.6, 0.85, 10, 0.35,
    "From raw battery cycler data to a deployable TFLite model  (Google Colab + PyProBE + TensorFlow)",
    sz=14, c=LGRAY)

# ─── Left: Data Processing ───
rrect(s, 0.5, 1.5, 6.0, 5.5, BG_CARD)
rrect(s, 0.5, 1.5, 6.0, 0.06, ACCENT)
txb(s, 0.7, 1.65, 5.6, 0.35, "Stage 1:  Battery Data Processing  (PyProBE)", sz=16, c=ACCENT, b=True)

data_steps = [
    ("1. Load Raw CSV",           DGRAY,  False,
     "Battery cycler test data (A_Test.csv)\nColumns: Time, Step, Current, Voltage,\nCapacity, Temperature"),
    ("2. PyProBE Column Mapping",  DGRAY,  False,
     "CastAndRenameMap maps cycler columns\nto standard PyProBE names (polars)"),
    ("3. Process to Parquet",      DGRAY,  False,
     "process_cycler_data() converts CSV\nto optimised Parquet format"),
    ("4. Create Cell Object",      DGRAY,  False,
     "Cell(NMC622, 20Ah) + import_data()\nSegments: charge, discharge, rest"),
    ("5. Compute SOC",            RGBColor(0x1B,0x5E,0x20), True,
     "proc.set_SOC() — Coulomb counting\nbased on nominal capacity"),
    ("6. Compute SOH",            RGBColor(0x1B,0x5E,0x20), True,
     "cycling.summary() — capacity fade\nper cycle vs nominal capacity"),
    ("7. Export Labelled CSV",     RGBColor(0x00,0x60,0x64), True,
     "A_Test_processed.csv with columns:\nVoltage, Current, Temp, SOC%, SOH%"),
]
y = 2.15
for txt, clr, is_new, desc in data_steps:
    rrect(s, 0.7, y, 2.6, 0.42, clr, txt, sz=10,
          tc=GREEN if is_new else WHITE, b=is_new)
    txb(s, 3.4, y, 2.9, 0.55, desc, sz=9, c=LGRAY)
    if y < 2.15 + 0.52 * 6:
        arrow(s, 2.0, y + 0.42, 2.0, y + 0.52, CYAN)
    y += 0.52

# ─── Right: Model Training ───
rrect(s, 6.8, 1.5, 6.1, 5.5, BG_CARD)
rrect(s, 6.8, 1.5, 6.1, 0.06, GREEN)
txb(s, 7.0, 1.65, 5.7, 0.35, "Stage 2:  Neural Network Training  (TensorFlow / Keras)", sz=16, c=GREEN, b=True)

# Dataset card
rrect(s, 7.0, 2.2, 5.7, 1.0, RGBColor(0x1A,0x23,0x7E))
mtxb(s, 7.1, 2.25, 5.5, 0.9, [
    ("DATASET", YELLOW, True, 13),
    ("Input X:   Voltage [V],  Current [A],  Temperature [C]   (3 features)", WHITE, False, 11),
    ("Output y:  SOC [%],  SOH [%]   (2 targets)", GREEN, False, 11),
    ("Split:     80% train  /  20% test   (random_state=42)", LGRAY, False, 11),
])

# Architecture card
rrect(s, 7.0, 3.4, 5.7, 2.1, RGBColor(0x1B,0x5E,0x20))
mtxb(s, 7.1, 3.45, 5.5, 2.0, [
    ("MODEL ARCHITECTURE  (Sequential)", YELLOW, True, 13),
    ("", None, False, 4),
    ("Input(shape=3)                   — raw V, I, T", WHITE, False, 11),
    ("Normalization()                  — StandardScaler inside model", WHITE, False, 11),
    ("Reshape((3,1))                   — sequence format for CNN", WHITE, False, 11),
    ("Conv1D(32, kernel=2, relu)       — feature extraction", CYAN, False, 11),
    ("GlobalAveragePooling1D()         — reduce to fixed vector", CYAN, False, 11),
    ("Dense(64, relu)                  — hidden layer 1", WHITE, False, 11),
    ("Dense(32, relu)                  — hidden layer 2", WHITE, False, 11),
    ("Dense(2)                         — output: SoC, SoH", GREEN, False, 11),
])

# Training card
rrect(s, 7.0, 5.7, 2.7, 1.1, RGBColor(0x00,0x60,0x64))
mtxb(s, 7.1, 5.75, 2.5, 1.0, [
    ("TRAINING", YELLOW, True, 12),
    ("Optimizer:  Adam", WHITE, False, 11),
    ("Loss:  MSE", WHITE, False, 11),
    ("Epochs:  10", WHITE, False, 11),
    ("Batch:  32", WHITE, False, 11),
])

# Output card
rrect(s, 10.0, 5.7, 2.7, 1.1, RGBColor(0x4A,0x14,0x8C))
mtxb(s, 10.1, 5.75, 2.5, 1.0, [
    ("OUTPUT", YELLOW, True, 12),
    ("NET_model.tflite", CYAN, True, 11),
    ("Metrics: MSE, RMSE,", WHITE, False, 10),
    ("MAE, MAPE, R-squared", WHITE, False, 10),
])

slide_number_footer(s, 3, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")


# =====================================================================
#  SLIDE 4 — STM32Cube.AI CONVERSION
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 10, 0.6, "STM32Cube.AI  -  Model Deployment to Microcontroller", sz=30, c=WHITE, b=True)
txb(s, 0.6, 0.85, 10, 0.35,
    "Converting TFLite model to optimised C code for STM32 Cortex-M0+",
    sz=14, c=LGRAY)

# ─── Horizontal pipeline ───
pipe_items = [
    (".tflite\nModel",     RGBColor(0x4A,0x14,0x8C)),
    ("STM32Cube.AI\nAnalyse", RGBColor(0x1A,0x23,0x7E)),
    ("Quantise &\nOptimise",  RGBColor(0xBF,0x36,0x0C)),
    ("Generate\nC / H Files", RGBColor(0x1B,0x5E,0x20)),
    ("STM32CubeIDE\nBuild",   RGBColor(0x00,0x60,0x64)),
    ("Flash to\nSTM32 MCU",   DGRAY),
]
cx = 0.6
for txt, clr in pipe_items:
    chevron(s, cx, 1.5, 2.0, 0.9, clr, txt, sz=12, tc=WHITE, b=True)
    cx += 2.1

# ─── Left: STM32Cube.AI Process ───
rrect(s, 0.5, 2.7, 6.0, 4.3, BG_CARD)
rrect(s, 0.5, 2.7, 6.0, 0.06, ACCENT)
txb(s, 0.7, 2.85, 5.6, 0.35, "STM32Cube.AI Tool Process", sz=16, c=ACCENT, b=True)

cubeai_steps = [
    ("1. Import Model",
     "Load NET_model.tflite into\nSTM32Cube.AI (X-CUBE-AI plugin)",
     RGBColor(0x4A,0x14,0x8C)),
    ("2. Analyse",
     "Validate model compatibility with target MCU\nEstimate RAM, Flash, MACC requirements",
     RGBColor(0x1A,0x23,0x7E)),
    ("3. Validate (Optional)",
     "Run on-desktop validation with test dataset\nCompare model output vs ground truth",
     DGRAY),
    ("4. Generate Code",
     "Produce optimised C source + headers:\n  network.c / network.h\n  network_data.c / network_data.h",
     RGBColor(0x1B,0x5E,0x20)),
    ("5. Integrate in Project",
     "Add generated files to STM32CubeIDE project\nLink with ST Edge AI runtime library",
     RGBColor(0x00,0x60,0x64)),
]
y = 3.35
for title, desc, clr in cubeai_steps:
    rrect(s, 0.7, y, 2.4, 0.5, clr, title, sz=11, tc=WHITE, b=True)
    txb(s, 3.2, y, 3.1, 0.65, desc, sz=10, c=LGRAY)
    if y < 3.35 + 0.62 * 4:
        arrow(s, 1.9, y + 0.5, 1.9, y + 0.62, CYAN)
    y += 0.62

# ─── Right: Generated Files & Runtime ───
rrect(s, 6.8, 2.7, 6.1, 4.3, BG_CARD)
rrect(s, 6.8, 2.7, 6.1, 0.06, GREEN)
txb(s, 7.0, 2.85, 5.7, 0.35, "Generated Files & Runtime Integration", sz=16, c=GREEN, b=True)

# Files card
rrect(s, 7.0, 3.35, 5.7, 1.8, RGBColor(0x1A,0x23,0x7E))
mtxb(s, 7.1, 3.4, 5.5, 1.7, [
    ("FILES GENERATED BY STM32Cube.AI", YELLOW, True, 13),
    ("", None, False, 4),
    ("network.c / network.h          Model topology & inference API", WHITE, False, 11),
    ("network_data.c / .h            Weights, biases (const arrays)", WHITE, False, 11),
    ("app_x-cube-ai.c / .h           Application wrapper & init", CYAN, False, 11),
    ("", None, False, 4),
    ("RUNTIME LIBRARY (libai)", YELLOW, True, 13),
    ("stai.h                         ST Edge AI runtime API", WHITE, False, 11),
    ("stai_network_run()             Execute inference", GREEN, False, 11),
    ("stai_input[] / stai_output[]   Model I/O buffers", GREEN, False, 11),
])

# Integration card
rrect(s, 7.0, 5.35, 5.7, 1.5, RGBColor(0x1B,0x5E,0x20))
mtxb(s, 7.1, 5.4, 5.5, 1.4, [
    ("HOW app_bms_demo_ai_v2.c USES THESE FILES", YELLOW, True, 13),
    ("", None, False, 4),
    ("#include \"app_x-cube-ai.h\"    — AI middleware entry points", WHITE, False, 11),
    ("#include \"network.h\"          — Model-specific defines", WHITE, False, 11),
    ("#include \"stai.h\"             — Runtime API", WHITE, False, 11),
    ("", None, False, 4),
    ("AI_Init()  calls  STM32CubeAI_Studio_AI_Init()", CYAN, False, 11),
    ("AI_RunInference()  writes stai_input, calls aiRun(), reads stai_output", GREEN, False, 11),
])

slide_number_footer(s, 4, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")


# =====================================================================
#  SLIDE 5 — SYSTEM ARCHITECTURE
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 8, 0.6, "System Architecture", sz=30, c=WHITE, b=True)
txb(s, 0.6, 0.85, 10, 0.35,
    "End-to-end signal path from battery cells to intelligent SoC/SoH estimation",
    sz=14, c=LGRAY)

# Pipeline — horizontal chevrons
pipe = [
    ("L9961\nBMS IC",       RGBColor(0x1A, 0x23, 0x7E)),
    ("Sensor\nData Read",   DGRAY),
    ("Data\nConversion",    DGRAY),
    ("AI Neural\nNetwork",  RGBColor(0x1B, 0x5E, 0x20)),
    ("Saturate\n[0, 100]",  RGBColor(0xBF, 0x36, 0x0C)),
    ("Moving\nAverage",     RGBColor(0x00, 0x60, 0x64)),
    ("UART\nOutput",        RGBColor(0x4A, 0x14, 0x8C)),
]
cx = 0.6
for txt, clr in pipe:
    chevron(s, cx, 1.5, 1.75, 0.9, clr, txt, sz=11, tc=WHITE, b=True)
    cx += 1.8

# Detail cards — row 1
card_data = [
    ("Hardware",  "L9961 BMS IC monitors 5 battery cells\nvia SPI on STM32 Nucleo board",
     [("5x Cell Voltages", "mV"), ("Instantaneous Current", "mA"),
      ("Die Temperature", "deg C"), ("NTC Thermistor", "mV"),
      ("Battery Sum Voltage", "mV")],
     RGBColor(0x1A, 0x23, 0x7E), 0.6),
    ("AI Engine", "ST Edge AI middleware deploys a\ntrained neural network on-device",
     [("Framework", "ST Edge AI (X-CUBE-AI)"), ("Runtime", "stai_network_run()"),
      ("Input Shape", "3 x float32"), ("Output Shape", "2 x float32"),
      ("Inference Rate", "~300ms cycle")],
     RGBColor(0x1B, 0x5E, 0x20), 4.7),
    ("Post-Processing", "Raw AI output is sanitised and\nsmoothed before display",
     [("Saturation", "Clamp to [0%, 100%]"), ("Filter", "10-sample Moving Avg"),
      ("Buffer Type", "Circular (ring)"), ("Warm-up", "1..10 sample ramp"),
      ("Output", "Raw + Averaged values")],
     RGBColor(0x00, 0x60, 0x64), 8.8),
]
for title, desc, items, clr, lx in card_data:
    rrect(s, lx, 2.8, 3.8, 4.2, BG_CARD)
    rrect(s, lx, 2.8, 3.8, 0.06, clr)
    txb(s, lx + 0.15, 2.95, 3.5, 0.35, title, sz=18, c=clr, b=True)
    txb(s, lx + 0.15, 3.35, 3.5, 0.55, desc, sz=11, c=LGRAY)
    y = 4.05
    for label, val in items:
        txb(s, lx + 0.15, y, 1.7, 0.25, label, sz=11, c=WHITE, b=True)
        txb(s, lx + 1.9, y, 1.7, 0.25, val, sz=11, c=CYAN)
        y += 0.3

slide_number_footer(s, 5, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")

# =====================================================================
#  SLIDE 6 — CODE FLOW
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 8, 0.6, "Software Code Flow", sz=30, c=WHITE, b=True)
txb(s, 0.6, 0.85, 10, 0.35,
    "Initialization sequence and periodic main-loop execution",
    sz=14, c=LGRAY)

# ─── Left: Init ───
rrect(s, 0.5, 1.5, 3.8, 5.3, BG_CARD)
rrect(s, 0.5, 1.5, 3.8, 0.06, ACCENT)
txb(s, 0.7, 1.65, 3.4, 0.35, "Startup  (APP_BMS_Init + Start)", sz=15, c=ACCENT, b=True)

init_steps = [
    ("ComInit(UART)",             DGRAY,                      False),
    ("BSP_L9961_Init()",          DGRAY,                      False),
    ("AI_Init()  — NEW",          RGBColor(0x1B,0x5E,0x20),   True),
    ("APP_BMS_Conf()  config",    DGRAY,                      False),
    ("Interrupt Init + Fault",    DGRAY,                      False),
    ("Draw UI Header",            DGRAY,                      False),
    ("Enable Output Switch",      DGRAY,                      False),
]
y = 2.15
for i, (txt, clr, is_new) in enumerate(init_steps):
    rrect(s, 0.8, y, 3.2, 0.42, clr, f"{i+1}. {txt}", sz=11,
          tc=GREEN if is_new else WHITE, b=is_new)
    if i < len(init_steps) - 1:
        arrow(s, 2.4, y + 0.42, 2.4, y + 0.52, CYAN)
    y += 0.52

# ─── Center: Main Loop ───
rrect(s, 4.7, 1.5, 4.0, 5.3, BG_CARD)
rrect(s, 4.7, 1.5, 4.0, 0.06, GREEN)
txb(s, 4.9, 1.65, 3.6, 0.35, "Main Loop  (APP_BMS_Task)", sz=15, c=GREEN, b=True)

loop_steps = [
    ("CheckDevice()  — board OK?",     DGRAY,                     False),
    ("DataRead()  — V, I, T sensors",  RGBColor(0x1A,0x23,0x7E),  False),
    ("AI_RunInference(V,I,T) — NEW",   RGBColor(0x1B,0x5E,0x20),  True),
    ("AI_UpdateMovingAvg() — NEW",     RGBColor(0x00,0x60,0x64),  True),
    ("Output  (UI / Tab / CSV)",       RGBColor(0x4A,0x14,0x8C),  False),
    ("ManageFault()",                  DGRAY,                     False),
    ("CommandMgr + Button Handler",    DGRAY,                     False),
]
y = 2.15
for i, (txt, clr, is_new) in enumerate(loop_steps):
    rrect(s, 5.0, y, 3.4, 0.42, clr, txt, sz=11,
          tc=GREEN if is_new else WHITE, b=is_new)
    if i < len(loop_steps) - 1:
        arrow(s, 6.7, y + 0.42, 6.7, y + 0.52, CYAN)
    y += 0.52

txb(s, 5.0, y + 0.15, 3.4, 0.25, "Repeats every ~300ms", sz=11, c=YELLOW, b=True, al=PP_ALIGN.CENTER)

# ─── Right: AI Detail ───
rrect(s, 9.1, 1.5, 3.8, 5.3, BG_CARD)
rrect(s, 9.1, 1.5, 3.8, 0.06, ORANGE)
txb(s, 9.3, 1.65, 3.4, 0.35, "AI Inference Detail", sz=15, c=ORANGE, b=True)

# Input
rrect(s, 9.3, 2.2, 3.4, 1.3, RGBColor(0x1A,0x23,0x7E))
mtxb(s, 9.4, 2.25, 3.2, 1.2, [
    ("MODEL INPUT  (3 floats)", YELLOW, True, 12),
    ("", None, False, 4),
    ("[0]  Cell1 Voltage  (V)",   WHITE, False, 11),
    ("[1]  Current  (mA)",        WHITE, False, 11),
    ("[2]  Die Temperature  (C)", WHITE, False, 11),
])
arrow(s, 11.0, 3.55, 11.0, 3.75, GREEN, Pt(3))

# Model
rrect(s, 9.3, 3.8, 3.4, 0.55, RGBColor(0x1B,0x5E,0x20),
      "stai_network_run()", sz=14, tc=GREEN, b=True)
arrow(s, 11.0, 4.4, 11.0, 4.6, GREEN, Pt(3))

# Output
rrect(s, 9.3, 4.65, 3.4, 0.9, RGBColor(0x00,0x60,0x64))
mtxb(s, 9.4, 4.7, 3.2, 0.8, [
    ("MODEL OUTPUT  (2 floats)", YELLOW, True, 12),
    ("", None, False, 4),
    ("[0]  SoC  — State of Charge  (%)", GREEN, False, 11),
    ("[1]  SoH  — State of Health  (%)", GREEN, False, 11),
])
arrow(s, 11.0, 5.6, 11.0, 5.8, ORANGE, Pt(3))

# Post
rrect(s, 9.3, 5.85, 3.4, 0.8, RGBColor(0x4A,0x14,0x8C))
mtxb(s, 9.4, 5.9, 3.2, 0.7, [
    ("POST-PROCESSING", YELLOW, True, 12),
    ("1. Saturate to [0, 100]%",       WHITE, False, 11),
    ("2. Moving Average (10 samples)", CYAN,  False, 11),
    ("3. Display Raw + Avg values",    WHITE, False, 11),
])

slide_number_footer(s, 6, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")

# =====================================================================
#  SLIDE 7 — VT100 UI TERMINAL OUTPUT
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 8, 0.6, "Live Terminal Output  —  VT100 UI Mode", sz=30, c=WHITE, b=True)
txb(s, 0.6, 0.85, 10, 0.35,
    "Real-time display on Tera Term showing sensor data with AI-estimated SoC/SoH",
    sz=14, c=LGRAY)

# Simulated VT100 UI
ui_lines = [
    [(" STSW-L9961BMS Demo AI 1.0.0 (August 9th, 2026)                ", "white", "blue_bg")],
    [],
    [("  Mode           : ", "white"), ("Normal", "white")],
    [("  Read Count     : ", "white"), ("182", "white")],
    [("  Tick       (ms): ", "white"), ("55208", "white")],
    [("  Output         : ", "white"), (" On  ", "white", "green_bg")],
    [("  Fault          : ", "white"), ("  N  ", "white", "green_bg")],
    [],
    [("  Cell1      (mV): ", "white"), ("3684", "cyan")],
    [("  Cell2      (mV): ", "white"), ("3683", "cyan")],
    [("  Cell3      (mV): ", "white"), ("3680", "cyan")],
    [("  Cell4      (mV): ", "white"), ("3694", "cyan")],
    [("  Cell5      (mV): ", "white"), ("3685", "cyan")],
    [("  SumMea     (mV): ", "white"), ("18422", "cyan")],
    [("  Current    (mA): ", "white"), ("1", "cyan")],
    [("  NTC        (mV): ", "white"), ("2487", "yellow")],
    [("  DieTemp  (degC): ", "white"), ("29", "yellow")],
    [("  AI SoC       (%): ", "green"), ("94.57", "green")],
    [("  AI SoH       (%): ", "green"), ("100.00", "green")],
    [("  Avg SoC      (%): ", "cyan"),  ("93.82", "cyan")],
    [("  Avg SoH      (%): ", "cyan"),  ("99.45", "cyan")],
    [(" Copyright (C) 2026                                            ", "white", "blue_bg")],
]
add_terminal_image(s, 0.6, 1.4, 6.0, ui_lines, "COM3 - Tera Term VT  |  VT100 UI Mode")

# Annotations on right
txb(s, 7.0, 1.4, 5.5, 0.4, "Display Breakdown", sz=18, c=ACCENT, b=True)

annotations = [
    ("Header Bar", "Blue background with firmware name,\nversion, and date", ACCENT, 1.9),
    ("System Status", "Mode, read count, tick counter,\noutput switch (green=ON), fault pin", WHITE, 2.8),
    ("Cell Voltages", "5 individual cell voltages in mV (cyan)\nplus sum measurement", CYAN, 3.7),
    ("Current & Temp", "Instantaneous current (cyan),\nNTC and die temperature (yellow)", YELLOW, 4.5),
    ("AI Raw Output", "Direct neural network SoC/SoH\npredictions in green", GREEN, 5.3),
    ("AI Filtered Output", "10-sample moving average of\nsaturated SoC/SoH in cyan", CYAN, 5.95),
    ("Footer Bar", "Copyright info on blue background", ACCENT, 6.6),
]
for title, desc, clr, ty in annotations:
    rrect(s, 7.0, ty, 0.12, 0.35, clr)
    txb(s, 7.3, ty, 2.2, 0.35, title, sz=13, c=clr, b=True)
    txb(s, 9.5, ty, 3.5, 0.45, desc, sz=10, c=LGRAY)

slide_number_footer(s, 7, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")


# =====================================================================
#  SLIDE 8 — TAB & CSV OUTPUT MODES
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 8, 0.6, "Data Logging Modes  —  Tab & CSV", sz=30, c=WHITE, b=True)
txb(s, 0.6, 0.85, 10, 0.35,
    "Two additional output formats for data acquisition, analysis, and export",
    sz=14, c=LGRAY)

# Tab mode terminal
tab_header = [
    [("Mode     ", "white", "blue_bg"), ("Count ", "white", "blue_bg"),
     ("Tick(ms) ", "white", "blue_bg"), (" Out ", "white", "blue_bg"),
     (" Flt ", "white", "blue_bg"),
     ("VCell1 ", "white", "blue_bg"), ("VCell2 ", "white", "blue_bg"),
     ("VCell3 ", "white", "blue_bg"), ("VCell4 ", "white", "blue_bg"),
     ("VCell5 ", "white", "blue_bg"),
     ("SumMea ", "white", "blue_bg"),
     (" Curr ", "white", "blue_bg"),
     (" NTC  ", "white", "blue_bg"),
     ("DTemp ", "white", "blue_bg"),
     ("AI SoC", "white", "green_bg"),
     (" AI SoH", "white", "green_bg"),
     (" AvgSoC", "white", "green_bg"),
     (" AvgSoH", "white", "green_bg")],
]
tab_data_rows = []
import random
random.seed(42)
for i in range(6):
    rc = 180 + i
    tick = 55000 + i * 305
    soc = 93.5 + random.uniform(-0.5, 0.5)
    soh = 99.2 + random.uniform(-0.3, 0.3)
    asoc = 93.8 + random.uniform(-0.2, 0.2)
    asoh = 99.4 + random.uniform(-0.1, 0.1)
    row = [
        (f"Normal   ", "white"), (f" {rc}  ", "white"),
        (f" {tick}  ", "white"), (" On  ", "white", "green_bg"),
        ("  N  ", "white", "green_bg"),
        (f" 3684 ", "cyan"), (f" 3681 ", "cyan"), (f" 3680 ", "cyan"),
        (f" 3694 ", "cyan"), (f" 3685 ", "cyan"),
        (f" 18422", "cyan"),
        (f"    1 ", "cyan"),
        (f" 2487 ", "yellow"),
        (f"  29  ", "yellow"),
        (f" {soc:5.2f} ", "green"),
        (f" {soh:5.2f} ", "green"),
        (f" {asoc:5.2f} ", "cyan"),
        (f" {asoh:5.2f} ", "cyan"),
    ]
    tab_data_rows.append(row)

add_terminal_image(s, 0.4, 1.4, 12.5, tab_header + tab_data_rows,
                   "COM3 - Tera Term VT  |  Tab Log Mode (U+2)")

# CSV mode
csv_lines = [
    [("Mode,ReadCount,Tick(ms),Elapsed(ms),Output,Fault,Cell1,Cell2,Cell3,Cell4,Cell5,"
      "CellSum,VBatt,Current,NTC,DieTemp,AI_SoC,AI_SoH,Avg_SoC,Avg_SoH,AI_Valid", "green")],
    [("Normal,180,55208,305,1,N,3684,3681,3680,3694,3685,18422,18415,1,2487,29,"
      "94.57,100.00,93.82,99.45,1", "white")],
    [("Normal,181,55513,305,1,N,3684,3683,3680,3694,3685,18422,18415,1,2487,29,"
      "93.91,100.00,93.76,99.41,1", "white")],
    [("Normal,182,55818,305,1,N,3684,3681,3680,3694,3685,18422,18415,1,2487,29,"
      "94.12,100.00,93.80,99.43,1", "white")],
]
add_terminal_image(s, 0.4, 4.5, 12.5, csv_lines,
                   "COM3 - Tera Term VT  |  CSV Log Mode (U+3)")

# Mode descriptions
txb(s, 0.6, 6.6, 5.5, 0.4, "Tab Mode (U+2):  Colour-coded fixed-width columns for visual monitoring",
    sz=12, c=LGRAY)
txb(s, 6.8, 6.6, 6.0, 0.4, "CSV Mode (U+3):  Comma-separated output for import into Excel / MATLAB / Python",
    sz=12, c=LGRAY)

slide_number_footer(s, 8, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")


# =====================================================================
#  SLIDE 9 — SUMMARY & NEXT STEPS
# =====================================================================
s = prs.slides.add_slide(prs.slide_layouts[6]); bg(s)
rrect(s, 0, 0, 13.333, 0.08, ACCENT)

txb(s, 0.6, 0.3, 8, 0.6, "Summary & Next Steps", sz=30, c=WHITE, b=True)

# Key results
rrect(s, 0.5, 1.2, 6.0, 2.8, BG_CARD)
rrect(s, 0.5, 1.2, 6.0, 0.06, GREEN)
txb(s, 0.7, 1.35, 5.6, 0.35, "What We Delivered", sz=18, c=GREEN, b=True)

results = [
    ("On-device AI inference", "SoC and SoH estimated in real-time on Cortex-M0+", GREEN),
    ("Production-grade output", "Saturated [0-100%] and filtered (10-sample moving avg)", CYAN),
    ("3 output modes", "VT100 UI, Tab Log, CSV Log — all with AI data", ACCENT),
    ("Zero external dependencies", "No cloud, no PC compute — fully embedded solution", ORANGE),
    ("Backward compatible", "All Demo1 features retained (fault, pause, commands)", WHITE),
]
y = 1.85
for title, desc, clr in results:
    rrect(s, 0.7, y, 0.1, 0.3, clr)
    txb(s, 0.95, y, 2.5, 0.3, title, sz=13, c=clr, b=True)
    txb(s, 3.3, y, 3.0, 0.3, desc, sz=11, c=LGRAY)
    y += 0.35

# Technical specs
rrect(s, 6.9, 1.2, 6.0, 2.8, BG_CARD)
rrect(s, 6.9, 1.2, 6.0, 0.06, ACCENT)
txb(s, 7.1, 1.35, 5.6, 0.35, "Technical Specifications", sz=18, c=ACCENT, b=True)

specs = [
    ("MCU",             "STM32 Cortex-M0+"),
    ("BMS IC",          "L9961 — 5-cell monitor"),
    ("AI Framework",    "ST Edge AI (X-CUBE-AI)"),
    ("Model Input",     "3 floats: Voltage, Current, Temperature"),
    ("Model Output",    "2 floats: SoC (%), SoH (%)"),
    ("Inference Rate",  "Every 300ms data cycle"),
    ("Filter",          "10-sample Simple Moving Average"),
    ("Saturation",      "Clamped to [0%, 100%]"),
    ("Output",          "UART → Tera Term (VT100 / Tab / CSV)"),
]
y = 1.85
for label, val in specs:
    txb(s, 7.1, y, 2.3, 0.25, label, sz=12, c=WHITE, b=True)
    txb(s, 9.4, y, 3.3, 0.25, val, sz=12, c=CYAN)
    y += 0.28

# Next steps
rrect(s, 0.5, 4.3, 12.4, 2.5, BG_CARD)
rrect(s, 0.5, 4.3, 12.4, 0.06, ORANGE)
txb(s, 0.7, 4.45, 5.6, 0.35, "Recommended Next Steps", sz=18, c=ORANGE, b=True)

next_steps = [
    ("Multi-Cell Input", "Extend AI input from Cell1 only\nto all 5 cell voltages + NTC", GREEN, 0.7, 5.0),
    ("On-Board Validation", "Validate AI predictions against\nCoulomb-counting SoC on live battery pack", ACCENT, 4.6, 5.0),
    ("Cloud Logging", "Stream CSV data to cloud dashboard\nfor fleet-level battery analytics", CYAN, 8.5, 5.0),
]
for title, desc, clr, nx, ny in next_steps:
    rrect(s, nx, ny, 3.6, 1.5, RGBColor(0x1A, 0x20, 0x3A))
    rrect(s, nx, ny, 3.6, 0.05, clr)
    txb(s, nx + 0.15, ny + 0.15, 3.3, 0.3, title, sz=15, c=clr, b=True)
    txb(s, nx + 0.15, ny + 0.55, 3.3, 0.7, desc, sz=12, c=LGRAY)

slide_number_footer(s, 9, TOTAL_SLIDES, "STSW-L9961BMS AI Demo")


# ─────────────────────── remove metadata & save ───────────────────────
prs.core_properties.author = ""
prs.core_properties.title = ""
prs.core_properties.subject = ""
prs.core_properties.comments = ""
prs.core_properties.keywords = ""
prs.core_properties.category = ""
prs.core_properties.content_status = ""
prs.core_properties.description = ""
prs.core_properties.identifier = ""
prs.core_properties.language = ""
prs.core_properties.last_modified_by = ""
prs.core_properties.revision = 1
prs.core_properties.version = ""

output_path = os.path.join(DOCS_DIR, "AI_BMS_Demo_Overview.pptx")
prs.save(output_path)

# ─── Strip ALL private/confidential data from PPTX internals ───
import zipfile, shutil, tempfile, re

EMPTY_CUSTOM_XML = (
    '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
    '<Properties xmlns="http://schemas.openxmlformats.org/officeDocument/2006/custom-properties"'
    ' xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"/>'
)

def _scrub_core_xml(raw: bytes) -> bytes:
    """Remove personal info from docProps/core.xml."""
    text = raw.decode("utf-8")
    text = re.sub(r"(<dc:creator>)[^<]*(</dc:creator>)", r"\1\2", text)
    text = re.sub(r"(<cp:lastModifiedBy>)[^<]*(</cp:lastModifiedBy>)", r"\1\2", text)
    text = re.sub(r"(<cp:revision>)[^<]*(</cp:revision>)", r"\g<1>1\2", text)
    return text.encode("utf-8")

def _scrub_app_xml(raw: bytes) -> bytes:
    """Remove Company/Manager from docProps/app.xml."""
    text = raw.decode("utf-8")
    text = re.sub(r"(<Company>)[^<]*(</Company>)", r"\1\2", text)
    text = re.sub(r"(<Manager>)[^<]*(</Manager>)", r"\1\2", text)
    return text.encode("utf-8")

tmp = tempfile.NamedTemporaryFile(delete=False, suffix=".pptx")
tmp.close()
with zipfile.ZipFile(output_path, "r") as zin, zipfile.ZipFile(tmp.name, "w") as zout:
    for item in zin.infolist():
        data = zin.read(item.filename)
        if item.filename == "docProps/custom.xml":
            data = EMPTY_CUSTOM_XML.encode("utf-8")
        elif item.filename == "docProps/core.xml":
            data = _scrub_core_xml(data)
        elif item.filename == "docProps/app.xml":
            data = _scrub_app_xml(data)
        zout.writestr(item, data)
shutil.move(tmp.name, output_path)

print(f"Saved: {output_path}")
