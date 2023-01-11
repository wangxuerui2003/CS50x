export function checkTaskListener() {
	let checkButtons = document.querySelectorAll(".check-task");

	checkButtons.forEach(function (btn) {
		btn.addEventListener('change', function () {
			let form = document.createElement('form');
			form.setAttribute('action', '/check_task');
			form.setAttribute('method', 'post');
			form.appendChild(btn);
			document.body.appendChild(form);
			form.submit();

		})
	})
}

checkTaskListener();