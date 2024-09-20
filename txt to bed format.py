import re


def txt_to_bed(input_txt, output_bed):
    with open(input_txt, 'r') as txt_file, open(output_bed, 'w') as bed_file:
        lines = txt_file.readlines()

        i = 0
        while i < len(lines):
            if lines[i].startswith('>'):
                header = lines[i].strip()
                chrom, start, end, name, score = parse_header(header)


                strand = "."

                bed_line = f"{chrom}\t{start}\t{end}\t{name}\t{score}\t{strand}\n"
                bed_file.write(bed_line)

                
                i += 1
                while i < len(lines) and not lines[i].startswith('>'):
                    i += 1
            else:
                i += 1


def parse_header(header):
    parts = header.split('|')
    chrom = parts[1]
    start = parts[2]
    end = parts[4]
    name = f"mite|{chrom}|{start}|{end}|t{parts[6]}|m{parts[8]}"
    score_match = re.search(r'ave_score:(\d+\.\d+)', header)
    score = score_match.group(1) if score_match else "0"
    return chrom, start, end, name, score


input_txt = '\output.txt'  # 输入的TXT文件路径
output_bed = '\output.bed'  # 输出的BED文件路径

txt_to_bed(input_txt, output_bed)
