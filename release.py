#!/usr/bin/python3
from argparse import ArgumentParser as ap

def main(source: str, header: str, output: str) -> None:
    content = ''

    with open(header, 'r') as hfile:
        for hline in hfile:
            if not hline.strip().startswith('#endif // __MIDCXXAP_HPP_INCLUDED'):
                content += hline
                continue

            to_write = ''
            with open(source, 'r') as srcfile:
                to_write = srcfile.read()
            
            content += to_write + '\n'
            del to_write

            content += hline
            break
    
    with open(output, 'w') as outfile:
        outfile.write(content)

if __name__ == "__main__":
    parser = ap(prog="release.py", description="Makes an usable single-header release for midccxxap.")

    parser.add_argument("-s", "--source", required=True)
    parser.add_argument("-i", "--header", required=True)
    parser.add_argument("-o", "--output", required=True)

    args = parser.parse_args()

    main(args.source, args.header, args.output)