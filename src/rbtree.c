#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void)
{
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    // sentinel로 쓸 nil노드 초기화
    node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
    nil_node->parent = nil_node;
    nil_node->left = nil_node;
    nil_node->right = nil_node;
    nil_node->color = RBTREE_BLACK;
    // 루트노드 초기화
    p->root = nil_node;
    p->nil = nil_node;
    return p;
}

node_t *new_red_node(rbtree *t, const key_t key)
{
    node_t *n = (node_t *)malloc(sizeof(node_t));
    n->key = key;
    n->left = t->nil;
    n->right = t->nil;
    n->parent = t->nil;
    n->color = RBTREE_RED;
    return n;
}

void delete_node(node_t *start, rbtree *t)
{
    // nil_node면
    if (start == t->nil)
    {
        return;
    }
    // 리프노드일 경우
    else if (start->left == t->nil && start->right == t->nil)
        free(start);
    // 후위순회로 삭제
    else
    {
        if (start->left != t->nil)
            delete_node(start->left, t);
        if (start->right != t->nil)
            delete_node(start->right, t);
        free(start);
    }
}
void delete_rbtree(rbtree *t)
{
    // TODO: reclaim the tree nodes's memory
    // 포함된 노드들 제거
    delete_node(t->root, t);
    // sentinel한 nil노드 제거
    free(t->nil);
    free(t);
}

void rotation_left(rbtree *t, node_t *target)
{
    // left_child 설정
    node_t *y = target->right;

    // y의 왼쪽 서브트리를 오른쪽 서브트리로 붙인다.-------------
    target->right = y->left;
    // 만약 서브트리가 nil 노드가 아니라면
    // 서브트리의 부모를 target 노드로 갱신해준다.
    if (y->left != t->nil)
        y->left->parent = target;

    // target의 부모를 y로 연결한다.------------------------
    y->parent = target->parent;
    // 만약 target이 루트노드라면
    if (target->parent == t->nil)
        t->root = y;
    // 루트노드가 아니라면 부모의 어느쪽 자식 노드인지
    // 왼쪽이라면
    else if (target == target->parent->left)
        target->parent->left = y;
    // 오른쪽이라면
    else
        target->parent->right = y;

    // target을 y의 왼쪽으로 연결한다.----------------------
    y->left = target;
    target->parent = y;
}
void rotation_right(rbtree *t, node_t *target)
{
    node_t *y = target->left;
    target->left = y->right;
    if (y->left != t->nil)
        y->right->parent = target;
    y->parent = target->parent;
    if (target->parent == t->nil)
        t->root = y;
    else if (target == target->parent->left)
        target->parent->left = y;
    else
        target->parent->right = y;
    y->right = target;
    target->parent = y;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{

    node_t *new_node = new_red_node(t, key);

    // 노드 추가될 자리 찾기
    node_t *x = t->root; // key 비교용 노드
    node_t *y = t->nil;  // 추가된 노드의 부모가 될 노드
    while (x != t->nil)
    {
        y = x;
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    new_node->parent = y;

    // 만약 비어있는 트리였다면 루트로 설정
    if (y == t->nil)
        t->root = new_node;
    // 아니라면 key값을 통해 이동한 위치에 연결
    else if (new_node->key < y->key)
        y->left = new_node;
    else
        y->right = new_node;

    // 추가 후 balancing
    rbtree_insert_fixup(t, new_node);

    return new_node;
}

void rbtree_insert_fixup(rbtree *t, node_t *z)
{
    // 부모노드의 색이 빨강이고
    while (z->parent->color == RBTREE_RED)
    {
        // 만약 부모 노드가 할아버지 노드의 왼쪽 자식이라면
        if (z->parent == z->parent->parent->left)
        {
            node_t *y = z->parent->parent->right; // 삼촌노드 y

            // case.1 부모,삼촌 다 빨강이라면
            if (y->color == RBTREE_RED)
            {
                // 부모,삼촌과 할아버지의 색깔 교환
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;

                // 대상 노드를 할아버지로 바꿔서 연쇄적으로 확인
                z = z->parent->parent;
            }
            // case.2 부모는 빨강, 삼촌은 검정이라면
            else
            {
                // case.2-1 부모는 할아버지의 왼쪽 + 대상 노드는 부모의 오른쪽
                // 즉, 꺽여있다면
                // 왼,왼 만들기 위해 부모기준 왼쪽 회전
                if (z == z->parent->right)
                {
                    // 회전하면 z는 어차피 대상(손자)노드를 가리키게 된다.
                    z = z->parent;
                    rotation_left(t, z);
                }

                // 위 코드를 지났다면 왼,왼인 상태
                //  색 바꾸고 할아버지 기준 오른쪽 회전
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                rotation_right(t, z->parent->parent);
            }
        }
        // 부모가 할아버지의 오른쪽 자식이라면
        // 위 코드와 구성 동일, 좌우반전
        else
        {
            node_t *y = z->parent->parent->left;
            if (y->color == RBTREE_RED)
            {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rotation_right(t, z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                rotation_left(t, z->parent->parent);
            }
        }
    }
    // 끝으로 루트노드 검정으로 고정
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_min(const rbtree *t)
{
    // TODO: implement find
    node_t *temp = t->root;
    node_t *nil = t->nil;
    // 트리가 비어있으면
    if (temp == nil)
        return nil;

    // 아니면
    while (1)
    {
        // 왼쪽이 자식이 없는 노드 반환
        if (temp->left == nil)
            return temp;
        // 있으면 왼쪽 이동
        temp = temp->left;
    }
}
node_t *rbtree_max(const rbtree *t)
{
    node_t *temp = t->root;
    node_t *nil = t->nil;
    if (temp == nil)
        return nil;
    while (1)
    {
        if (temp->right == nil)
            return temp;
        temp = temp->right;
    }
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
    // TODO: implement find
    // TODO: implement find
    node_t *temp = t->root;

    // 트리가 비어있다면

    // 아니라면
    while (1)
    {
        // 못 찾았으면
        if (temp == t->nil)
        {
            return NULL;
        }
        // 같은 값 찾으면 반환
        if (temp->key == key)
        {
            return temp;
        }
        // 현재 노드보다 key값이 작다면
        if (temp->key > key)
        {
            // 왼쪽으로 이동
            temp = temp->left;
        }
        // 크다면
        else
        {
            temp = temp->right;
        }
    }
}

void inorder_travel(const rbtree *t, node_t *start, key_t *arr, int *i, int N)
{
    // 비어있거나, N번 반복했다면
    if (start == t->nil || *i == N)
    {
        return;
    }
    // 리프노드일 경우
    else if (start->left == t->nil && start->right == t->nil)
    {
        arr[*i] = start->key;
        *i += 1;
    }
    // 중위순회
    else
    {
        if (start->left != t->nil)
            inorder_travel(t, start->left, arr, i, N);
        arr[*i] = start->key;
        *i += 1;
        if (start->right != t->nil)
            inorder_travel(t, start->right, arr, i, N);
    }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
    // TODO: implement to_array
    node_t *start = t->root;
    int idx = 0;
    int *i = &idx;
    inorder_travel(t, start, arr, i, n);
    return 0;
}

// TODO ---------------- under

// 자식이 한개만 있는 경우 자식과 조부모를 연결
void transplant(rbtree *t, node_t *a, node_t *b)
{
    if (a->parent == t->nil)
        t->root = b;
    // 부모가 조부모의 왼쪽 자식이었으면
    else if (a == a->parent->left)
        a->parent->left = b;
    // 부모가 조부모의 오른쪽 자식이었으면
    else
        a->parent->right = b;
    b->parent = a->parent;
}

int rbtree_erase(rbtree *t, node_t *p)
{
    // balancing할 때 extra black 이 붙어야 하는 노드 저장할 변수
    node_t *x;

    // 자식 0개,1개일 경우난 자기색이 색 없어지기 때문에
    // 없어지는 노드의 색 초기값은 자기 색깔
    node_t *y = p;
    color_t y_origin_color = y->color;

    // 오른쪽 자식만 있거나 자식이 없으면
    if (p->left == t->nil)
    {
        x = p->right;
        transplant(t, p, p->right);
    }
    // 왼쪽 자식만 있거나 자식이 없으면
    else if (p->right == t->nil)
    {
        x = p->left;
        transplant(t, p, p->left);
    }
    // 자식이 2개라면
    // 없어지는 색은 successor의 색이 된다.
    else
    {
        // successor 찾기
        node_t *temp = p->right;
        while (temp->left != t->nil)
        {
            temp = temp->left;
        }
        y = temp;
        y_origin_color = y->color;
        // successor는 왼쪽 자식은 없으니 오른쪽 자식만 확인
        x = y->right;

        // 삭제대상 오른쪽 자식이 successor라면
        if (y == p->right)
            x->parent = y;
        // 오 -> 왼 -> 왼...으로 successor를 찾았다면
        else
        {
            // 오른쪽 자식 있든 없든 조부모랑 연결
            transplant(t, y, y->right);
            y->right = p->right;
            y->right->parent = y;
        }
        transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }
    // 연결이 완료되었다면 삭제대상 노드를 메모리 해제해준다.
    free(p);
    // 삭제시 balancing은 삭제된 노드의 색이 검은  색일 경우에만 하면 된다.
    if (y_origin_color == RBTREE_BLACK)
        rbtree_delete_fixup(t, x);
    return 0;
}
void rbtree_delete_fixup(rbtree *t, node_t *x)
{
    // 시작시 x의 색은 black
    // x가 루트이면서 black일 때, 혹은 x가 red가 되면 종료
    // x의 형제노드를 저장할 노드 w
    node_t *w;
    while (x != t->root && x->color == RBTREE_BLACK)
    {
        // x가 부모의 왼쪽 자식이라면
        if (x == x->parent->left)
        {
            // x의 형제노드 w
            w = x->parent->right;
            // Case.1 형제의 색이 빨강이라면 -> 자식은 무조건 다 black -----------------------
            // Case.2,3,4중 하나로 만들기 위해 회전시키고 형제를 갱신하는 작업을 실행한다.
            if (w->color == RBTREE_RED)
            {
                // 형제와 부모의 색을 바꾸고 회전
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotation_left(t, x->parent);
                // 회전시 w는 x의 부모노드가 된다.
                // 다시 형제노드로 지정해주기
                w = x->parent->right;
            }

            // Case.2 형제가 black이고 조카노드들도 다 black인 경우 -------------------------
            // Case.1을 지나왔기 때문에 형제는 반드시 black이다.
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
            {
                // 형제의 색을 red로 바꿔주고
                w->color = RBTREE_RED;
                // x를 갱신함으로써 다시 Case들을 확인하게 한다.
                x = x->parent;
            }

            // 조카노드가 B,B가 아니라면
            else
            {
                // Case.3 형제는 black, 오른쪽 조카 black , 왼쪽 조카 red --------------------
                // 오른쪽이 black이라면 왼쪽은 상위 조건문에 의해 red
                if (w->right->color == RBTREE_BLACK)
                {
                    // Case.4를 만들기 위해 형제노드 기준 색 바꾸고 오른쪽 회전
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotation_right(t, w);
                    // 회전했으니 형제노드 다시 갱신
                    w = x->parent->right;
                }

                // Case.4 형제는 black , 오른쪽 조카 red , 왼쪽 조카는 상관없다.-----------------
                // Case.3을 지나왔기 때문에 오른쪽 조카는 반드시 red가 된다.
                // 부모와 형제의 색 바꾸고 회전
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                rotation_left(t, x->parent);
                x = t->root;
            }
        }
        // 부모의 오른쪽 자식이라면
        // 왼쪽 코드 좌,우 반전시키면 된다. (rotation함수도 반전시켜야 한다.)
        else
        {
            w = x->parent->left;
            if (w->color == RBTREE_RED)
            {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rotation_right(t, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
            {
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == RBTREE_BLACK)
                {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rotation_left(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                rotation_right(t, x->parent);
                x = t->root;
            }
        }
    }
    // 반복문을 지나면 대상노드x는 루트노드이거나, Red and black의 색을 가진 노드가 된다.
    // 색을 검정으로 바꿔주고 balancing을 종료한다.
    x->color = RBTREE_BLACK;
}
