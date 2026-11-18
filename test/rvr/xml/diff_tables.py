import re
import sys

def parse(path):
    tables = {}
    with open(path, encoding="utf-8") as f:
        content = f.read()
    for m in re.finditer(r'<table name="([^"]+)"[^>]*address="([0-9A-Fa-f]+)"[^>]*type="(2D|3D)"', content):
        name, addr = m.group(1), m.group(2).upper().lstrip("0") or "0"
        tables.setdefault(name, []).append(addr)
    return tables

exported_path = sys.argv[1]
current_path = sys.argv[2]

exported = parse(exported_path)
current = parse(current_path)

diffs = []
for name, exp_addrs in exported.items():
    cur_addrs = current.get(name)
    if cur_addrs is None:
        continue
    for ea in exp_addrs:
        if ea not in cur_addrs:
            diffs.append((name, cur_addrs, exp_addrs))

for name, cur, exp in diffs:
    print("%-70s cur=%-10s exp=%-10s" % (name, ",".join(cur), ",".join(exp)))
print("Total diffs:", len(diffs))
