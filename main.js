const MAX_OPERATOR_LEVEL = 3;
var max = {
    val: -Infinity,
    s: ""
}; /* max_val, max_s */

function StringToNumber(s) {
    let number = parseFloat(s.s.substring(s.idx));

    if (s.s[s.idx] == '-') {
        s.idx += 1;
    }

    while (((s.s[s.idx] >= '0') && (s.s[s.idx] <= '9')) || s.s[s.idx] == '.') {
        s.idx += 1;
    }

    return number;
}

function EvaluateTerm(s) {
    if (s.s[s.idx] == '(') {
        s.idx += 1;
        let temp = s.idx;
        let pcnt = 1;

        while (pcnt > 0) {
            if (s.s[s.idx] == '(') {
                pcnt += 1;
            }
            else if (s.s[s.idx] == ')') {
                pcnt -= 1;
            }
            s.idx += 1;
        }

        let ss = {
            s: "",
            idx: 0
        }
        ss.s = s.s.substring(temp, s.idx - 1);

        return CalculateExpression(ss);
    }
    else {
        return StringToNumber(s);
    }
}

function OperatorPriority(op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case 'X':
        case 'x':
        case '/':
            return 2;
        case '^':
            return 3;
    }
}

function CalculateOperator(left, right, op) {
    switch (op) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
        case 'X':
        case 'x':
            return left * right;
        case '/':
            return left / right;
        case '^':
            return Math.pow(left, right);
    }
}

function CalculateExpression(s, level = 1) {
    if (level > MAX_OPERATOR_LEVEL) {
        return EvaluateTerm(s);
    }

    let left, right, op;

    left = CalculateExpression(s, level + 1);
    op = s.s[s.idx];

    while ((s.idx < s.s.length) && (OperatorPriority(op) == level)) {
        s.idx += 1;

        right = CalculateExpression(s, level + 1);

        left = CalculateOperator(left, right, op);

        if (s.idx < s.s.length) {
            op = s.s[s.idx];
        } else {
            return left;
        }
    }

    return left;
}

function FindSpace(s) {
    for (let i = s.idx; i < s.s.length; i++) {
        if (s.s[i] == ' ') {
            return i;
        }
    }

    return s.s.length;
}

function Solve(s, add, sub, mul, div, pow, seq_pow) {
    let new_idx = FindSpace(s);

    if (s.idx === 0) {
        max.val = -Infinity;
        seq_pow = false;
    }

    if (s.s.length == new_idx) {
        let temp = s.idx;
        s.idx = 0;

        let value = CalculateExpression(s);

        if (isFinite(value)) {
            if (value > max.val) {
                max.val = value;
                max.s = s.s.slice();
            }
        }

        s.idx = temp;
    } else {
        let temp = s.idx;
        s.idx = new_idx;

        if (add > 0) {
            s.s = s.s.substring(0, new_idx) + '+' + s.s.substring(new_idx + 1);
            Solve(s, add - 1, sub, mul, div, pow, false);
        }

        if (sub > 0) {
            s.s = s.s.substring(0, new_idx) + '-' + s.s.substring(new_idx + 1);
            Solve(s, add, sub - 1, mul, div, pow, false);
        }

        if (mul > 0) {
            s.s = s.s.substring(0, new_idx) + '*' + s.s.substring(new_idx + 1);
            Solve(s, add, sub, mul - 1, div, pow, false);
        }

        if (div > 0) {
            s.s = s.s.substring(0, new_idx) + '/' + s.s.substring(new_idx + 1);
            Solve(s, add, sub, mul, div - 1, pow, false);
        }

        if ((pow > 0) && !seq_pow) {
            s.s = s.s.substring(0, new_idx) + '^' + s.s.substring(new_idx + 1);
            Solve(s, add, sub, mul, div, pow - 1, true);
        }

        s.s = s.s.substring(0, new_idx) + ' ' + s.s.substring(new_idx + 1);
        s.idx = temp; /* reset for adjacent trees */
    }
}

(function () {
    var s = {
        s: "",
        idx: 0
    };

    s.s = prompt("Enter your equation:");
    alert(s.s);
    if (s.s == "exit") return;

    var es = prompt("Number of add sub mul div pow: (splitted by blank)");
    es = es.split(" ");
    var add = es[0], sub = es[1], mul = es[2], div = es[3], pow = es[4];

    s.idx = 0;
    Solve(s, add, sub, mul, div, pow, false);

    if (isFinite(max.val)) {
        alert("Maximum Value: " + max.val + '\n' + "Expression: " + max.s);
    } else {
        alert("Error while evaluating!");
    } 
})();