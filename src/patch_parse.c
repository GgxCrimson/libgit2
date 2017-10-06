#include "patch_parse.h"
#define parse_err(...) \
	( giterr_set(GITERR_PATCH, __VA_ARGS__), -1 )


GIT_INLINE(bool) parse_ctx_contains(
	git_patch_parse_ctx *ctx, const char *str, size_t len)
{
	return (ctx->line_len >= len && memcmp(ctx->line, str, len) == 0);
}

#define parse_ctx_contains_s(ctx, str) \
	parse_ctx_contains(ctx, str, sizeof(str) - 1)

static void parse_advance_line(git_patch_parse_ctx *ctx)
{
	ctx->line += ctx->line_len;
	ctx->remain_len -= ctx->line_len;
	ctx->line_len = git__linenlen(ctx->line, ctx->remain_len);
	ctx->line_num++;
}

static void parse_advance_chars(git_patch_parse_ctx *ctx, size_t char_cnt)
{
	ctx->line += char_cnt;
	ctx->remain_len -= char_cnt;
	ctx->line_len -= char_cnt;
}

static int parse_advance_expected(
	git_patch_parse_ctx *ctx,
	const char *expected,
	size_t expected_len)
{
	if (ctx->line_len < expected_len)
		return -1;

	if (memcmp(ctx->line, expected, expected_len) != 0)
		return -1;

	parse_advance_chars(ctx, expected_len);
	return 0;
}

#define parse_advance_expected_str(ctx, str) \
	parse_advance_expected(ctx, str, strlen(str))

static int parse_advance_ws(git_patch_parse_ctx *ctx)
{
	int ret = -1;

	while (ctx->line_len > 0 &&
		ctx->line[0] != '\n' &&
		git__isspace(ctx->line[0])) {
		ctx->line++;
		ctx->line_len--;
		ctx->remain_len--;
		ret = 0;
	}

	return ret;
}

static int parse_advance_nl(git_patch_parse_ctx *ctx)
{
	if (ctx->line_len != 1 || ctx->line[0] != '\n')
		return -1;

	parse_advance_line(ctx);
	return 0;
}

	bool quoted = (ctx->line_len > 0 && ctx->line[0] == '"');
	for (len = quoted; len < ctx->line_len; len++) {
		if (!quoted && git__isspace(ctx->line[len]))
		else if (quoted && !inquote && ctx->line[len] == '"') {
		inquote = (!inquote && ctx->line[len] == '\\');
static int parse_header_path_buf(git_buf *path, git_patch_parse_ctx *ctx)
	int path_len, error = 0;

	path_len = header_path_len(ctx);
	if ((error = git_buf_put(path, ctx->line, path_len)) < 0)
	parse_advance_chars(ctx, path_len);
	int error = parse_header_path_buf(&path, ctx);
	return parse_header_path(&patch->old_path, ctx);
	return parse_header_path(&patch->new_path, ctx);
	const char *end;
	int32_t m;
	int ret;

	if (ctx->line_len < 1 || !git__isdigit(ctx->line[0]))
		return parse_err("invalid file mode at line %"PRIuZ, ctx->line_num);
	if ((ret = git__strntol32(&m, ctx->line, ctx->line_len, &end, 8)) < 0)
		return ret;
	parse_advance_chars(ctx, (end - ctx->line));

	return ret;
	for (len = 0; len < ctx->line_len && len < GIT_OID_HEXSZ; len++) {
		if (!git__isxdigit(ctx->line[len]))
		git_oid_fromstrn(oid, ctx->line, len) < 0)
		return parse_err("invalid hex formatted object id at line %"PRIuZ,
			ctx->line_num);
	parse_advance_chars(ctx, len);
		parse_advance_expected_str(ctx, "..") < 0 ||
	if (ctx->line_len > 0 && ctx->line[0] == ' ') {
		parse_advance_chars(ctx, 1);
	if (parse_header_path_buf(&path, ctx) < 0)
	int32_t val;
	const char *end;
	if (ctx->line_len < 1 || !git__isdigit(ctx->line[0]) ||
		git__strntol32(&val, ctx->line, ctx->line_len, &end, 10) < 0)
	parse_advance_chars(ctx, (end - ctx->line));

	if (parse_advance_expected_str(ctx, "%") < 0)
	if (val > 100)
		return parse_err("invalid similarity percentage at line %"PRIuZ,
			ctx->line_num);
		return parse_err("invalid similarity percentage at line %"PRIuZ,
			ctx->line_num);
} header_git_op;

static const header_git_op header_git_ops[] = {
	{ "diff --git ", NULL },
	{ "@@ -", NULL },
	{ "GIT binary patch", NULL },
	{ "Binary files ", NULL },
	{ "--- ", parse_header_git_oldpath },
	{ "+++ ", parse_header_git_newpath },
	{ "index ", parse_header_git_index },
	{ "old mode ", parse_header_git_oldmode },
	{ "new mode ", parse_header_git_newmode },
	{ "deleted file mode ", parse_header_git_deletedfilemode },
	{ "new file mode ", parse_header_git_newfilemode },
	{ "rename from ", parse_header_renamefrom },
	{ "rename to ", parse_header_renameto },
	{ "rename old ", parse_header_renamefrom },
	{ "rename new ", parse_header_renameto },
	{ "copy from ", parse_header_copyfrom },
	{ "copy to ", parse_header_copyto },
	{ "similarity index ", parse_header_similarity },
	{ "dissimilarity index ", parse_header_dissimilarity },

	/* Parse the diff --git line */
	if (parse_advance_expected_str(ctx, "diff --git ") < 0)
		return parse_err("corrupt git diff header at line %"PRIuZ, ctx->line_num);

	if (parse_header_path(&patch->header_old_path, ctx) < 0)
		return parse_err("corrupt old path in git diff header at line %"PRIuZ,
			ctx->line_num);

	if (parse_advance_ws(ctx) < 0 ||
		parse_header_path(&patch->header_new_path, ctx) < 0)
		return parse_err("corrupt new path in git diff header at line %"PRIuZ,
			ctx->line_num);
	for (parse_advance_line(ctx);
		ctx->remain_len > 0;
		parse_advance_line(ctx)) {

		if (ctx->line_len == 0 || ctx->line[ctx->line_len - 1] != '\n')
		for (i = 0; i < ARRAY_SIZE(header_git_ops); i++) {
			const header_git_op *op = &header_git_ops[i];
			size_t op_len = strlen(op->str);
			if (memcmp(ctx->line, op->str, min(op_len, ctx->line_len)) != 0)
			if (op->fn == NULL)
			parse_advance_chars(ctx, op_len);
			if ((error = op->fn(patch, ctx)) < 0)
			parse_advance_ws(ctx);
			if (parse_advance_expected_str(ctx, "\n") < 0 ||
			    ctx->line_len > 0) {
				error = parse_err("trailing data at line %"PRIuZ, ctx->line_num);
		
			error = parse_err("invalid patch header at line %"PRIuZ,
				ctx->line_num);
	if (!git__isdigit(ctx->line[0]))
	if (git__strntol64(&num, ctx->line, ctx->line_len, &end, 10) < 0)
	parse_advance_chars(ctx, (end - ctx->line));
	if (parse_number(&num, ctx) < 0 || !git__is_int(num))
	const char *header_start = ctx->line;
	if (parse_advance_expected_str(ctx, "@@ -") < 0 ||
	if (ctx->line_len > 0 && ctx->line[0] == ',') {
		if (parse_advance_expected_str(ctx, ",") < 0 ||
	if (parse_advance_expected_str(ctx, " +") < 0 ||
	if (ctx->line_len > 0 && ctx->line[0] == ',') {
		if (parse_advance_expected_str(ctx, ",") < 0 ||
	if (parse_advance_expected_str(ctx, " @@") < 0)
	parse_advance_line(ctx);
	hunk->hunk.header_len = ctx->line - header_start;
		return parse_err("oversized patch hunk header at line %"PRIuZ,
			ctx->line_num);
		ctx->line_num);
		ctx->remain_len > 1 &&
		(ctx->remain_len <= 4 || memcmp(ctx->line, "@@ -", 4) != 0);
		parse_advance_line(ctx)) {
		if (ctx->line_len == 0 || ctx->line[ctx->line_len - 1] != '\n') {
			error = parse_err("invalid patch instruction at line %"PRIuZ,
				ctx->line_num);
		switch (ctx->line[0]) {
			error = parse_err("invalid patch hunk at line %"PRIuZ, ctx->line_num);
		line->content = ctx->line + prefix;
		line->content_len = ctx->line_len - prefix;
		line->content_offset = ctx->content_len - ctx->remain_len;
		error = parse_err(
	if (parse_ctx_contains_s(ctx, "\\ ") &&
			error = parse_err("cannot trim trailing newline of empty line");
		parse_advance_line(ctx);
	for (ctx->line = ctx->remain;
		ctx->remain_len > 0;
		parse_advance_line(ctx)) {

		if (ctx->line_len < 6)
		if (parse_ctx_contains_s(ctx, "@@ -")) {
			size_t line_num = ctx->line_num;
			error = parse_err("invalid hunk header outside patch at line %"PRIuZ,
		if (ctx->remain_len < ctx->line_len + 6)
		if (parse_ctx_contains_s(ctx, "diff --git ")) {
	if (parse_ctx_contains_s(ctx, "literal ")) {
		parse_advance_chars(ctx, 8);
	} else if (parse_ctx_contains_s(ctx, "delta ")) {
		parse_advance_chars(ctx, 6);
		error = parse_err(
			"unknown binary delta type at line %"PRIuZ, ctx->line_num);
	if (parse_number(&len, ctx) < 0 || parse_advance_nl(ctx) < 0 || len < 0) {
		error = parse_err("invalid binary size at line %"PRIuZ, ctx->line_num);
	while (ctx->line_len) {
		char c = ctx->line[0];
			error = parse_err("invalid binary length at line %"PRIuZ, ctx->line_num);
		parse_advance_chars(ctx, 1);
		if (encoded_len > ctx->line_len - 1) {
			error = parse_err("truncated binary data at line %"PRIuZ, ctx->line_num);
			&decoded, ctx->line, encoded_len, decoded_len)) < 0)
			error = parse_err("truncated binary data at line %"PRIuZ, ctx->line_num);
		parse_advance_chars(ctx, encoded_len);
		if (parse_advance_nl(ctx) < 0) {
			error = parse_err("trailing data at line %"PRIuZ, ctx->line_num);
	git_buf_free(&base85);
	git_buf_free(&decoded);
	if (parse_advance_expected_str(ctx, "GIT binary patch") < 0 ||
		parse_advance_nl(ctx) < 0)
		return parse_err("corrupt git binary header at line %"PRIuZ, ctx->line_num);
	if (parse_advance_nl(ctx) < 0)
		return parse_err("corrupt git binary separator at line %"PRIuZ,
			ctx->line_num);
	if (parse_advance_nl(ctx) < 0)
		return parse_err("corrupt git binary patch separator at line %"PRIuZ,
			ctx->line_num);
	if (parse_advance_expected_str(ctx, "Binary files ") < 0 ||
		parse_advance_expected_str(ctx, patch->header_old_path) < 0 ||
		parse_advance_expected_str(ctx, " and ") < 0 ||
		parse_advance_expected_str(ctx, patch->header_new_path) < 0 ||
		parse_advance_expected_str(ctx, " differ") < 0 ||
		parse_advance_nl(ctx) < 0)
		return parse_err("corrupt git binary header at line %"PRIuZ, ctx->line_num);
	while (parse_ctx_contains_s(ctx, "@@ -")) {
	if (parse_ctx_contains_s(ctx, "GIT binary patch"))
	else if (parse_ctx_contains_s(ctx, "Binary files "))
		return parse_err("expected %s path of '/dev/null'", old_or_new);
		return parse_err("mismatched %s path names", old_or_new);
		return parse_err(
		return parse_err("missing new path");
		return parse_err("missing old path");
		return parse_err("git diff header lacks old / new paths");
		return parse_err("patch with no hunks");
	if (content_len) {
		if ((ctx->content = git__malloc(content_len)) == NULL) {
			git__free(ctx);
			return NULL;
		}

		memcpy((char *)ctx->content, content, content_len);
	ctx->content_len = content_len;
	ctx->remain = ctx->content;
	ctx->remain_len = ctx->content_len;

	git__free((char *)ctx->content);
	start = ctx->remain_len;
	used = start - ctx->remain_len;
	ctx->remain += used;
	GIT_REFCOUNT_INC(patch);