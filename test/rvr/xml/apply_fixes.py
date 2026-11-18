import re
import sys
import datetime

def parse(path):
    tables = {}
    with open(path, encoding="utf-8") as f:
        content = f.read()
    for m in re.finditer(r'<table name="([^"]+)"[^>]*address="([0-9A-Fa-f]+)"[^>]*type="(2D|3D)"', content):
        name, addr, ttype = m.group(1), m.group(2).upper(), m.group(3)
        tables.setdefault(name, []).append((addr, ttype))
    return tables

exported_path = sys.argv[1]
target_path = sys.argv[2]

exported = parse(exported_path)

with open(target_path, encoding="utf-8") as f:
    content = f.read()

changed = []
skipped = []

# Match each <table name="X" ... address="Y" ... type="2D"/"3D" ...> opening tag exactly.
pattern = re.compile(r'(<table name="([^"]+)"[^>]*?address=")([0-9A-Fa-f]+)("[^>]*type="(2D|3D)")')

def repl(m):
    prefix, name, cur_addr, suffix, ttype = m.groups()
    cur_addr_norm = cur_addr.upper()
    exp_list = exported.get(name)
    if not exp_list:
        return m.group(0)
    # find an exported address for this name that differs from current, with matching type
    for exp_addr, exp_type in exp_list:
        if exp_type == ttype and exp_addr.upper() != cur_addr_norm:
            hdr_size = 4 if ttype == "2D" else 7
            # sanity: difference should be exactly hdr_size
            if int(exp_addr, 16) - int(cur_addr, 16) == hdr_size:
                changed.append((name, cur_addr, exp_addr, ttype))
                return prefix + exp_addr + suffix
            else:
                skipped.append((name, cur_addr, exp_addr, ttype, "diff != hdr_size"))
                return m.group(0)
    return m.group(0)

new_content = pattern.sub(repl, content)

with open(target_path, "w", encoding="utf-8") as f:
    f.write(new_content)

print("Changed:", len(changed))
for c in changed:
    print("  %-70s %s -> %s (%s)" % c)
print("Skipped (mismatch):", len(skipped))
for s in skipped:
    print("  ", s)
