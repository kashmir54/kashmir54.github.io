from pygments.style import Style
from pygments.token import Token


class NightOwl(Style):
    background_color = '#011627'
    styles = {
        Token: '#d6deeb',
        Token.Keyword: 'italic #c792ea',
        Token.Keyword.Constant: 'noitalic #82aaff',
        Token.Keyword.Declaration: '',
        Token.Keyword.Namespace: '',
        Token.Keyword.Psuedo: '',
        Token.Keyword.Reserved: '',
        Token.Keyword.Type: 'noitalic #82aaff',
        Token.Name: '#d6deeb',
        Token.Name.Attribute: '#ff869a',
        Token.Name.Function: '#82aaff',
        Token.String.Quote: '#d9f5dd',
        Token.String: '#ecc48d',
        Token.Number: '#f78c6c',
        Token.Operator: '#7fdbca',
        Token.Comment: 'italic #637777',
        Token.Comment.Preproc: 'noitalic #7fdbca',
        Token.Generic.Strong: 'bold #addb67',
        Token.Generic.Emph: 'italic #697098',
        Token.Generic.Heading: '#82b1ff'
    }
