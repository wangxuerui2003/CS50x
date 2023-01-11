let resetButtons = document.querySelectorAll(".reset-button");

resetButtons.forEach(function (btn) {
	btn.addEventListener('click', function () {

		let task = btn.parentElement.querySelector("label").innerText;

		let form = document.createElement('form');
		let input = document.createElement('input');
		form.setAttribute('action', '/reset_task');
		form.setAttribute('method', 'post');
		input.setAttribute("name", "task");
		input.setAttribute("value", task);
		document.body.appendChild(form);
		form.appendChild(btn);
		form.appendChild(input);

		form.submit();

	})
});