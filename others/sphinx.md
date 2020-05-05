- Tutorial: https://www.youtube.com/watch?v=0ROZRNZkPS8
- `sphinx-quickstart` 
- `conf.py`
- `make clean && make build`
- `python3 -m http.server`
- `source format`
  - `reStructureText`
  - `Markdown`
    - `conf.py`

```python
import recommonmark.Parser

source_parsers = {
  '.md': 'recommonmark.parser.CommonMarkParser',
}

source_suffix = ['.rst', '.md']
```



- `Notebooks`

