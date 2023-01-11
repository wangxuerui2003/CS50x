function checkHabitListener() {
	let checkButtons = document.querySelectorAll(".check-habit");

	checkButtons.forEach(function (btn) {
		btn.addEventListener('change', function () {
			let form = document.createElement('form');
			form.setAttribute('action', '/check_habit');
			form.setAttribute('method', 'post');
			form.appendChild(btn);
			document.body.appendChild(form);
			form.submit();

		})
	})
}

checkHabitListener();