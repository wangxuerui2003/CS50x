let amount = 0;

document.addEventListener("DOMContentLoaded", function() {
    const confirm_form = document.querySelector("#confirm");
    buttons = document.querySelectorAll("button");
    for (const button of buttons) {
        button.onclick = function() {
            amount = button.value
            if (confirm_form.children.length == 0) {

                // Create the div element
                let div = document.createElement("div");
                div.setAttribute('class', 'input-group mx-auto w-auto');

                let el = document.createElement("input");
                el.setAttribute('class', 'form-control mx-auto w-auto');
                el.setAttribute('name', 'password');
                el.setAttribute('placeholder', 'Password');
                el.setAttribute('type', 'password');
                el.setAttribute('required', '');

                // Append the input form to the div
                div.appendChild(el);

                let bt = document.createElement("button");
                bt.setAttribute('class', 'btn btn-primary btn-md');
                bt.setAttribute('type', 'submit');
                bt.textContent = "Confirm"

                // Append the button to the div
                div.appendChild(bt);

                let prompt = document.createElement("h3");
                prompt.textContent = "Please input your password to confirm";

                confirm_form.appendChild(prompt);
                confirm_form.appendChild(div);
            }
        }
    }
});

document.addEventListener("submit", function() {
    const confirm_form = document.querySelector("#confirm");

    const submit_amount = document.createElement('input');
    submit_amount.setAttribute('name', 'amount');
    submit_amount.setAttribute('value', amount);
    submit_amount.setAttribute('type', 'hidden');

    confirm_form.appendChild(submit_amount);

    confirm_form.submit();
});
