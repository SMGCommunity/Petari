import json
import os

def generate_links(base_dir):
    folder_links = {}
    for root, dirs, files in os.walk(base_dir):
        md_files = [f for f in files if f.endswith(".md")]
        if md_files:
            folder_name = os.path.basename(root)
            folder_links[folder_name] = []
            for md_file in md_files:
                file_path = os.path.join(root, md_file)
                relative_path = os.path.relpath(file_path, base_dir)
                folder_links[folder_name].append(f"- [{md_file}](md/{relative_path.replace(os.sep, '/')})")

    return folder_links

with open("build\\RMGK01\\report.json", "r") as f:
    data = json.load(f)
    units = data["units"]

    for entry in units:
        metadata = entry.get("metadata", {})
        source_path = metadata.get("source_path")
        name = entry.get("name", "")

        filename = os.path.basename(source_path if source_path else name)
        if filename.startswith("auto"):
            continue

        if source_path:
            md_path = source_path.replace("src", "md").replace(".cpp", ".md").replace(".c", ".md")
        else:
            md_path = name.replace("main", "md") + ".md"

        os.makedirs(os.path.dirname(md_path), exist_ok=True)

        lines = [f"# {name}\n"]

        if source_path:
            lines.append(f"**Source Path:** `{source_path}`\n")

        lines.append("## Functions\n")
        lines.append("| Name | Address | Match % |")
        lines.append("|------|---------|---------|")

        for func in entry.get("functions", []):
            func_name = func.get("name", "Unnamed Function")
            address = func.get("metadata", {}).get("virtual_address")
            match = func.get("fuzzy_match_percent", 0.0)

            if address is not None:
                try:
                    address_int = int(address)
                    address_str = f"`0x{address_int:X}`"
                except ValueError:
                    address_str = "`0x0`"
            else:
                address_str = "`0x0`"

            if match == 100.0:
                match_emoji = ":white_check_mark:"
            else:
                match_emoji = ":x:"

            match_str = f"{match_emoji} ({match:.1f}%)"
            lines.append(f"| `{func_name}` | {address_str} | {match_str} |")

        with open(md_path, "w", encoding="utf-8") as md_file:
            md_file.write("\n".join(lines))

base_dir = "md"
folder_links = generate_links(base_dir)

with open("summary.md", "w", encoding="utf-8") as summary_file:
    summary_file.write("# Summary of Functions\n")
    for folder, links in folder_links.items():
        summary_file.write(f"\n## {folder}\n")
        for link in links:
            summary_file.write(link + "\n")
    
print("✅ Summary page with links generated.")
